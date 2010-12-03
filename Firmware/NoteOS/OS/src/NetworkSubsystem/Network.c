/*
 * Network.c
 *
 *  Created on: 10/09/2010
 *      Author: Coma
 */

#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "Network.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../Collections/Queue.h"
#include "../HardwareAbstractionLayer/HardwareAbstractionLayer.h"
#include "../MemorySubsystem/MemoryManager.h"
#include "../Diagnostics/Diagnostics.h"
#include "../SensorSubsystem/SensorManager.h"
#include "../BoardSupportPackage/BoardSupportPackage.h"
#include "../FixedMath.h"
#include "NetworkInternals.h"

static void* FrameReceived(void* data, uint8_t length);
static void UpdateCca();
static bool IsChannelClear();

// misc
//static unsigned long randomContext;

// link
#if MASTER_NODE == 1

volatile uint8_t linkState = LINK_STATE_IDLE;
uint8_t address = GATEWAY_ADDRESS;
static link_rts_packet rtsPacketTemplate = { .link.source = GATEWAY_ADDRESS, .link.type = TYPE_LINK_RTS };
static link_cts_packet ctsPacketTemplate = { .link.source = GATEWAY_ADDRESS, .link.type = TYPE_LINK_CTS };
static link_acknowledge_packet ackPacketTemplate = { .link.source = GATEWAY_ADDRESS, .link.type = TYPE_LINK_ACK };

#else

volatile uint8_t linkState = LINK_STATE_UNSYNCHRONIZED;
uint8_t address = BROADCAST_ADDRESS;
connection_state connectionState = CONNECTION_STATE_UNCONNECTED;
static link_rts_packet rtsPacketTemplate =
{	.link.type=TYPE_LINK_RTS};
static link_cts_packet ctsPacketTemplate =
{	.link.type=TYPE_LINK_CTS};
static link_acknowledge_packet ackPacketTemplate =
{	.link.type=TYPE_LINK_ACK};

#endif

static link_network_header* linkCurrentPacket;
static uint8_t linkCurrentPacketLength;
static uint8_t linkQueue[Queue_CalculateSize(sizeof(queue_element), NETWORK_LINK_QUEUE_SIZE)];
static uint8_t linkTransmissionAttemptCounter = 0;

void Network_Initialize()
{
	Queue_Initialize(linkQueue, sizeof(queue_element), NETWORK_LINK_QUEUE_SIZE);
	RddProtocol_Initialize();
}

void Network_Start()
{
	RadioDriver_SetFrameReceivedHandler(FrameReceived);

	srand(RadioDriver_GetRandomNumber());

	NetworkTimer_SetTimerValue(0);
}

#if MASTER_NODE == 0
static void AssignAddress(uint8_t adr)
{
	address = adr;
	rtsPacketTemplate.link.source = address;
	ctsPacketTemplate.link.source = address;
	ackPacketTemplate.link.source = address;
}
#endif

bool QueueLinkPacket(void* packet, uint8_t length)
{
	link_network_header* p = packet;

	if (Queue_IsFull(linkQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_LINK_QUEUE_OVERRUN);
		return false;
	}

	uint8_t nextNode = routingTable[linkCurrentPacket->network.receiver];
	if (nextNode == NO_ROUTE)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_STAR_TOPOLOGY_FALL_BACK);
		//return false;
		nextNode = linkCurrentPacket->network.receiver; // no route => go star
	}
	p->link.destination = nextNode;
	p->link.source = address;

	queue_element* qe = Queue_Head(linkQueue);
	qe->size = length;
	qe->object = p;
	Queue_AdvanceHead(linkQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_LINK_PACKET_QUEUED);

	return true;
}

// Internal

void SignalLinkLayerTransmissionAttemptFailed()
{
	if (++linkTransmissionAttemptCounter >= NETWORK_LINK_MAXIMUM_TRANSMISSION_ATTEMPTS)
	{
		linkTransmissionAttemptCounter = 0;
		if (Queue_IsEmpty(linkQueue) == false)
		{
			queue_element* qe = Queue_Tail(linkQueue);
			MemoryManager_Release(qe->object);
			Queue_AdvanceTail(linkQueue);
			Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_LINK_TX_PACKET_DROPPED);
		}
	}
}

/**
 * Called directly from the timer interrupt and so it is atomic.
 */
void Network_TimerEvent()
{
	SetBit(PORTF, 0);

	if (linkState == LINK_STATE_UNSYNCHRONIZED)
	{
		//	RadioDriver_EnableReceiveMode();
		ClearBit(PORTF, 0);
		return;
	}

	//Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_TICK);

	// update the CCA threshold
	//UpdateCca();


	// if state is not STATE_IDLE a frame was not received when one was expected
	switch (linkState)
	{
		case LINK_STATE_EXPECTING_RTS:
			// CCA indicated a transmission but no packet arrived. Note noisy environment.
			break;

		case LINK_STATE_EXPECTING_CTS:
			// RTS was never replied to. Inform router that the node addressed is bad. Maybe a weak link
			SignalLinkLayerTransmissionAttemptFailed();
			break;

		case LINK_STATE_EXPECTING_DATA:
			// Got RTS but no Data. Inform router of weak link.
			break;

		case LINK_STATE_EXPECTING_ACK:
			// Data was never acknowledged. Inform router of weak link.
			SignalLinkLayerTransmissionAttemptFailed();
			break;
	}

	// reset state
	linkState = LINK_STATE_IDLE;

	if (Queue_IsEmpty(linkQueue) == false) // frames to send?
	{
		//uint8_t slot = rand_r(&randomContext) & 0x3; // if this is a high priority node or packet choose an earlier slot
		uint8_t slot = 0;
		bool channelIsClear = true;
		for (uint8_t currentSlot = 0; currentSlot < slot; currentSlot++)
		{
			_delay_us(NETWORK_LINK_RTS_GUARD_SLOT_DURATION - NETWORK_LINK_CCA_CHECK_DURATION);

			if (IsChannelClear() == false)
			{
				channelIsClear = false;
				linkState = LINK_STATE_EXPECTING_RTS;
				break;
			}
		}

		if (channelIsClear == true)
		{
			// get next packet and find the next node in its route
			queue_element* qe = Queue_Tail(linkQueue);
			linkCurrentPacket = qe->object;
			linkCurrentPacketLength = qe->size;

			if (linkCurrentPacket->link.destination == BROADCAST_ADDRESS)
			{
				RadioDriver_SetTxPower(RADIODRIVER_TX_POWER_MAXIMUM); // broadcasts are always sent at maximum power.

				RadioDriver_Send(linkCurrentPacket, linkCurrentPacketLength); // send packet. stay in idle state as no reply is expected.

				MemoryManager_Release(linkCurrentPacket); // release allocated memory
				Queue_AdvanceTail(linkQueue); // and remove it from the queue

				Diagnostics_SendEvent(DIAGNOSTICS_TX_DATA);
			}
			else
			{
				// TODO: Extract destination and link loss for destination and calculate needed TX power level and set it. If NO_ROUTE use maximum power.
				RadioDriver_SetTxPower(RADIODRIVER_TX_POWER_MAXIMUM); // use maximum for now

				rtsPacketTemplate.link.destination = linkCurrentPacket->link.destination;
				rtsPacketTemplate.slot = slot;
				rtsPacketTemplate.transmissionPowerLevel = 0; // TODO Implement this
				RadioDriver_Send(&rtsPacketTemplate, sizeof(rtsPacketTemplate));

				linkState = LINK_STATE_EXPECTING_CTS;

				Diagnostics_SendEvent(DIAGNOSTICS_TX_RTS);
			}
		}
	}
	else
	{
		linkState = LINK_STATE_EXPECTING_RTS;


		//		for (uint8_t i = 0; i < NETWORK_LINK_NUMBER_OF_RTS_GUARD_SLOTS; i++)
		//		{
		//			if (IsChannelClear() == false) // transmission in progress
		//			{
		//				linkState = LINK_STATE_EXPECTING_RTS;
		//				break;
		//			}
		//			_delay_us(NETWORK_LINK_RTS_GUARD_SLOT_DURATION - NETWORK_LINK_CCA_CHECK_DURATION);
		//		}
		//
		//		if (linkState == LINK_STATE_IDLE) // channel clear => no one is sending
		//		{
		//			//RadioDriver_DisableReceiveMode();
		//		}
	}

	ClearBit(PORTF, 0);
}

void RawHandler(void* data, uint8_t length)
{
	Leds_RedToggle();
}

#if MASTER_NODE == 0
static void ConnectionHandler()
{
	if (connectionState == CONNECTION_STATE_CONNECTING)
	{
		application_join_request_packet* p = BedProtocol_CreatePacket(GATEWAY_ADDRESS, TYPE_APPLICATION_JOIN_REQUEST, sizeof(application_join_request_packet));
		if (p == NULL)
		{
			return;
		}

		//NonVolatileStorage_Read(0, p->serialNumber, sizeof(p->serialNumber));

		memcpy(p->serialNumber, serialNumber, sizeof(serialNumber));

		if (BedProtocol_QueuePacket())
		{
			Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_CREATED_JOIN_REQUEST);
		}
	}
}
#endif

#if MASTER_NODE == 1
static void JoinRequestHandler(void* data, uint8_t length)
{
	static uint8_t nextAddress = 1;

	application_join_request_packet* p = data;

	application_join_response_packet* response = BedProtocol_CreatePacket(BROADCAST_ADDRESS, TYPE_APPLICATION_JOIN_RESPONSE, sizeof(application_join_response_packet));
	if (response == NULL)
	{
		return;
	}

	response->assignedAddress = nextAddress;
	memcpy(response->serialNumber, p->serialNumber, lengthof(p->serialNumber));
	if (BedProtocol_QueuePacket())
	{
		nextAddress++;

		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_CREATED_JOIN_RESPONSE);

		Leds_GreenOn();
	}
}
#endif

#if MASTER_NODE == 0
static void JoinResponseHandler(void* data, uint8_t length)
{
	if (connectionState == CONNECTION_STATE_CONNECTING)
	{
		application_join_response_packet* p = data;

		//NonVolatileStorage_Read(0, p->serialNumber, sizeof(p->serialNumber));

		if (memcmp(serialNumber, p->serialNumber, sizeof(serialNumber)) == 0)
		{
			AssignAddress(p->assignedAddress);
			connectionState = CONNECTION_STATE_CONNECTED;
			EventDispatcher_Publish(EVENT_CONNECTED, NULL);
			Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_JOINED);
		}
	}
}
#endif

static void* FrameReceived(void* data, uint8_t length)
{
	SetBit(PORTF, 2);

	link_header* lh = data;

	switch (lh->type)
	{
		// link layer packets

		case TYPE_LINK_RTS:
			{
				link_rts_packet* p = data;

				NetworkTimer_SetTimerValue(17 + 0 * p->slot);

				if (linkState == LINK_STATE_UNSYNCHRONIZED)
				{
					linkState = LINK_STATE_EXPECTING_RTS;
				}

#if MASTER_NODE == 0
				if (connectionState == CONNECTION_STATE_UNCONNECTED)
				{
					if (EventDispatcher_Complete(ConnectionHandler))
					{
						connectionState = CONNECTION_STATE_CONNECTING;
					}
				}
#endif

				if (linkState != LINK_STATE_EXPECTING_RTS)
				{
					// signal unexpected RTS
					linkState = LINK_STATE_IDLE;
					break;
				}

				if (lh->destination != address)
				{
					break;
				}

				// TODO: Extract destination and link loss for destination and calculate needed TX power level and set it. If NO_ROUTE use maximum power.
				RadioDriver_SetTxPower(RADIODRIVER_TX_POWER_MAXIMUM); // use maximum for now

				ctsPacketTemplate.link.destination = lh->source;
				RadioDriver_Send(&ctsPacketTemplate, sizeof(ctsPacketTemplate));

				linkState = LINK_STATE_EXPECTING_DATA;

				Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_LINK_RTS_REPLIED);
			}
			break;

		case TYPE_LINK_CTS:
			{
				if (linkState != LINK_STATE_EXPECTING_CTS)
				{
					// signal unexpected CTS
					linkState = LINK_STATE_IDLE;
					break;
				}

				if (linkCurrentPacket->link.destination != lh->source)
				{
					// CTS from wrong node
					SignalLinkLayerTransmissionAttemptFailed();
					linkState = LINK_STATE_IDLE;
					break;
				}

				RadioDriver_Send(linkCurrentPacket, linkCurrentPacketLength);

				linkState = LINK_STATE_EXPECTING_ACK;

				Diagnostics_SendEvent(DIAGNOSTICS_TX_DATA);
			}
			break;

		case TYPE_LINK_ACK:
			{
				if (linkState != LINK_STATE_EXPECTING_ACK)
				{
					// signal unexpected ACK
					linkState = LINK_STATE_IDLE;
					break;
				}

				if (linkCurrentPacket->link.destination != lh->source)
				{
					// ACK from wrong node
					SignalLinkLayerTransmissionAttemptFailed();
					linkState = LINK_STATE_IDLE;
					break;
				}

				MemoryManager_Release(linkCurrentPacket); // release allocated memory
				Queue_AdvanceTail(linkQueue); // and remove it from the queue
				linkTransmissionAttemptCounter = 0;

				linkState = LINK_STATE_IDLE;

				Diagnostics_SendEvent(DIAGNOSTICS_RX_ACK);
			}
			break;


			// network layer packets

		case TYPE_NETWORK_ROUTES:
			{
				if (VerifyNetworkLayerHeader(&data, length))
				{
					//network_routes_packet* p = data;
					//memcpy(distances[p->network.sender], p->distances, 15);
				}
			}
			break;


			// transport layer packets

		case TYPE_APPLICATION_RAW_BED:
			BedProtocol_ProcessPacket(&data, length, RawHandler);
			break;

		case TYPE_TRANSPORT_RDD_ACK:
			{
				//				if (VerifyNetworkLayerHeader(&data, length))
				//				{
				//					queue_element* qe = Queue_Head(transportRddQueue);
				//					transport_rdd_acknowledge_packet* qp = qe->object;
				//					transport_rdd_acknowledge_packet* p = data;
				//					if (p->network.sender != qp->network.receiver || p->transport.sequenceNumber != qp->transport.sequenceNumber)
				//					{
				//						break;
				//					}
				//					// TODO: Send notification on timeout (acks just happens without a sound)
				//					Queue_AdvanceTail(transportRddQueue);
				//				}
			}
			break;


			// application layer packets

#if MASTER_NODE == 1
		case TYPE_APPLICATION_JOIN_REQUEST:
			BedProtocol_ProcessPacket(&data, length, JoinRequestHandler);
			break;
#endif


#if MASTER_NODE == 0
			case TYPE_APPLICATION_JOIN_RESPONSE:
			BedProtocol_ProcessPacket(&data, length, JoinResponseHandler);
			break;
#endif


			//		case TYPE_APPLICATION_SENSOR_DATA:
			//			if (RddProtocol_ProcessPacket(&data, length, sensorDataProcessor))
			//			{
			//				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			//			}
			//			break;

		case TYPE_APPLICATION_SET_PROPERTY_REQUEST:
			if (RddProtocol_ProcessPacket(&data, length, SensorManager_SetProperty))
			{
				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			}
			break;

		case TYPE_APPLICATION_GET_PROPERTY_REQUEST:
			if (RddProtocol_ProcessPacket(&data, length, SensorManager_GetProperty))
			{
				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			}
			break;

		default:
			linkState = LINK_STATE_IDLE;
			break;
	}

	ClearBit(PORTF, 2);
	return data;
}

bool VerifyNetworkLayerHeader(void** data, uint8_t length)
{
	link_header* lh = *data;
	link_network_header* lnh = *data;


	// link layer functionality

	// verify that a packet is expected
	if (linkState != LINK_STATE_EXPECTING_DATA)
	{
		linkState = LINK_STATE_IDLE;
		return false;
	}

	linkState = LINK_STATE_IDLE;


	// check address
	if (lh->destination != address && lh->destination != BROADCAST_ADDRESS)
	{
		return false;
	}

	if (lh->destination == address) // only ack unicasts
	{
		// send link layer acknowledge
		ackPacketTemplate.link.destination = lh->source;
		RadioDriver_Send(&ackPacketTemplate, sizeof(ackPacketTemplate));
	}

	// network layer functionality

	// forward if packet was not for this node
	if (lnh->network.receiver != address && lnh->network.receiver != BROADCAST_ADDRESS)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_FORWARDING);
		if (QueueLinkPacket(*data, length))
		{
			*data = NULL; // keep the allocated block
		}

		return false;
	}

	return true;
}

//
// Clear Channel Assessment stuff here

static int8_t ccaThreshold;

static void UpdateCca()
{
	SetBit(PORTF, 4);

	static uint8_t currentIndex = 0;
	static int8_t samples[NETWORK_LINK_CCA_SAMPLE_COUNT];
	static int16_t sum = 0;

	int8_t rssi = RadioDriver_GetRssi();

	sum -= samples[currentIndex];
	samples[currentIndex] = rssi;
	sum += rssi;

	if (++currentIndex >= NETWORK_LINK_CCA_SAMPLE_COUNT)
	{
		currentIndex = 0;
		Diagnostics_SendRaw(rssi);
	}

	static fixed15_16 oldMedian = 0;
	fixed15_16 newMedian = Fixed15_16Div(ToFixed15_16(sum), ToFixed15_16(NETWORK_LINK_CCA_SAMPLE_COUNT));
	static fixed15_16 oldThreshold = 0;
	fixed15_16 alpha = ToFixed15_16(NETWORK_LINK_CCA_ALPHA);
	fixed15_16 alphaMedian = Fixed15_16Mul(alpha, newMedian);
	fixed15_16 beta = Fixed15_16Sub(ToFixed15_16(1.0), alpha);
	fixed15_16 betaOld = Fixed15_16Mul(beta, oldThreshold);
	oldThreshold = Fixed15_16Add(alphaMedian, betaOld);

	oldMedian = newMedian;

	ccaThreshold = ToInt15_16(oldThreshold);


	//	static uint8_t reportCounter = 0;
	//	if (++reportCounter >= (128 / 10))
	//	{
	//		reportCounter = 0;
	//		Diagnostics_SendRaw(ccaThreshold);
	//	}


	//	Diagnostics_SendRaw(0);
	//	Diagnostics_SendRaw(1);
	//	Diagnostics_SendRaw(ccaThreshold * -1);
	//Diagnostics_SendRaw(rssi);
	//	Diagnostics_SendRaw(RadioDriver_GetRssiAtFrameStart() * -1);


	//	static float rssiSamples[NETWORK_LINK_CCA_SAMPLE_COUNT];
	//	static uint8_t rssiIndex = 0;
	//	static float oldMedian = 0;
	//	static float oldThreshold = 0;
	//	static float sum = 0;
	//
	//	float rssi = RadioDriver_GetRssi();
	//
	//	sum -= rssiSamples[rssiIndex];
	//	rssiSamples[rssiIndex] = rssi;
	//	sum += rssi;
	//
	//	if (++rssiIndex >= NETWORK_LINK_CCA_SAMPLE_COUNT)
	//	{
	//		rssiIndex = 0;
	//	}
	//
	//	float median = sum / (float) NETWORK_LINK_CCA_SAMPLE_COUNT;
	//
	//	oldThreshold = NETWORK_LINK_CCA_ALPHA * oldMedian + (1.0 - NETWORK_LINK_CCA_ALPHA) * oldThreshold;
	//
	//	oldMedian = median;
	//
	//	ccaThreshold = oldThreshold;

	ClearBit(PORTF, 4);
}

static bool IsChannelClear()
{
	SetBit(PORTF, 6);

	bool clear = false;


	// do all loops even though an outlier is found to maintain a constant execution time
	for (uint8_t i = 0; i < NETWORK_LINK_CCA_OUTLIER_COUNT; i++)
	{
		if (RadioDriver_GetRssi() < (ccaThreshold - NETWORK_LINK_CCA_THRESHOLD_SAFE_BAND))
		{
			clear = true;
		}
		_delay_us(NETWORK_LINK_CCA_CHECK_INTERVAL);
	}

	ClearBit(PORTF, 6);

	return clear;
}
