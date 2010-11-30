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

//  Constants
#define GATEWAY_ADDRESS																							0
#define BROADCAST_ADDRESS																						15

typedef enum
{
	LINK_STATE_UNSYNCHRONIZED,
	LINK_STATE_IDLE,
	LINK_STATE_EXPECTING_RTS,
	LINK_STATE_EXPECTING_CTS,
	LINK_STATE_EXPECTING_ACK,
	LINK_STATE_EXPECTING_DATA
} link_state;

// possible packet types
typedef enum
{
	// link packets
	TYPE_LINK_RTS,
	TYPE_LINK_CTS,
	TYPE_LINK_ACK,
	// network packets
	TYPE_NETWORK_ROUTES,
	// transport packets
	TYPE_TRANSPORT_RDD_ACK,
	// application packets
	TYPE_APPLICATION_JOIN_REQUEST,
	TYPE_APPLICATION_JOIN_RESPONSE,
	TYPE_APPLICATION_SENSOR_DATA,
	TYPE_APPLICATION_SET_PROPERTY_REQUEST,
	TYPE_APPLICATION_SET_PROPERTY_RESPONSE,
	TYPE_APPLICATION_GET_PROPERTY_REQUEST,
	TYPE_APPLICATION_GET_PROPERTY_RESPONSE,
// future packet types
//	TYPE_READ,
//	TYPE_READ_COMPLETE,
//	TYPE_WRITE,
//	TYPE_WRITE_COMPLETE
//	TYPE_LOAD,
//	TYPE_LOAD_COMPLETE,
//	TYPE_STORE,
//	TYPE_STORE_COMPLETE
} packet_type;

static void TransportRddTimeoutHandler();
static void* FrameReceived(void* data, uint8_t length);
static bool VerifyNetworkLayerHeader(void** data, uint8_t length);
static bool ProcessBedPacket(void** data, uint8_t length, block_handler packetHandler);
static bool ProcessRddPacket(void** data, uint8_t length, block_handler packetHandler);
static void UpdateCca();
static bool IsChannelClear();
static void InitiateSynchronization();
static bool QueueLinkPacket(link_network_header* packet, uint8_t length);

// misc
//static unsigned long randomContext;

// link
#if MASTER_NODE == 1

static volatile uint8_t linkState = LINK_STATE_IDLE;
static uint8_t address = GATEWAY_ADDRESS;
static link_rts_packet rtsPacketTemplate =
{	.link.source = GATEWAY_ADDRESS, .link.type = TYPE_LINK_RTS};
static link_cts_packet ctsPacketTemplate =
{	.link.source = GATEWAY_ADDRESS, .link.type = TYPE_LINK_CTS};
static link_acknowledge_packet ackPacketTemplate =
{	.link.source = GATEWAY_ADDRESS, .link.type = TYPE_LINK_ACK};

#else

static volatile uint8_t linkState = LINK_STATE_UNSYNCHRONIZED;
static uint8_t address = BROADCAST_ADDRESS;
static bool connected = false;
static link_rts_packet rtsPacketTemplate = { .link.type=TYPE_LINK_RTS };
static link_cts_packet ctsPacketTemplate = { .link.type=TYPE_LINK_CTS };
static link_acknowledge_packet ackPacketTemplate = { .link.type=TYPE_LINK_ACK };

#endif

static uint8_t linkCurrentSource;
static link_network_header* linkCurrentPacket;
static uint8_t linkCurrentPacketLength;
static uint8_t linkQueue[Queue_CalculateSize(sizeof(queue_element), NETWORK_LINK_QUEUE_SIZE)];
static uint8_t linkTransmissionCount = 0;

// network
#define NO_ROUTE																										0xff
static uint8_t routingTable[15] = { NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE };

#define NETWORK_UNREACHABLE																					0xffff
#define LINK_DEAD																										0

typedef struct
{
	int8_t loss;
	uint8_t node;
} route_hop;

typedef struct
{
	route_hop hops[15];
} route;

typedef struct
{
	uint16_t costToNeighbors[15];
	bool marked;
	uint16_t cost;
	uint8_t previousNode;
} node;

// transport
//   BED
static link_network_bed_transport_header* transportBedCurrentPacket;
static uint8_t transportBedCurrentPacketLength;
//   RDD
static uint8_t transportRddNextSequenceNumber = 0;
static uint8_t transportRddQueue[Queue_CalculateSize(sizeof(queue_element), NETWORK_TRANSPORT_RDD_QUEUE_SIZE)];
static link_network_rdd_transport_header* transportRddCurrentPacket;
static uint8_t transportRddCurrentPacketLength;
static uint8_t transportRddTimer;

void Network_Initialize()
{
	Queue_Initialize(linkQueue, sizeof(queue_element), NETWORK_LINK_QUEUE_SIZE);
	Queue_Initialize(transportRddQueue, sizeof(queue_element), NETWORK_TRANSPORT_RDD_QUEUE_SIZE);
}

void Network_Start()
{
	RadioDriver_SetFrameReceivedHandler(FrameReceived);

	srand(RadioDriver_GetRandomNumber());

	NetworkTimer_SetTimerPeriod(NETWORK_LINK_TIMER_FREQUENCY / NETWORK_LINK_COMMUNICATION_SLOT_FREQUENCY);
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

// Link

static bool QueueLinkPacket(link_network_header* packet, uint8_t length)
{
	if (Queue_IsFull(linkQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_LINK_QUEUE_OVERRUN);
		return false;
	}

	uint8_t nextNode = routingTable[linkCurrentPacket->network.receiver];
	if (nextNode == NO_ROUTE)
	{
		//return false;
		nextNode = linkCurrentPacket->network.receiver; // no route => go star
	}
	packet->link.destination = nextNode;
	packet->link.source = address;

	queue_element* qe = Queue_Head(linkQueue);
	qe->size = length;
	qe->object = packet;
	Queue_AdvanceHead(linkQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_LINK_PACKET_QUEUED);

	return true;
}

// End link

// BED protocol

void* Network_CreateBedPacket(uint8_t receiver, uint8_t type, uint8_t size)
{
	link_network_bed_transport_header* p = MemoryManager_Allocate(size);

	if (p == NULL)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_UNABLE_TO_CREATE_BED_PACKET);
		return NULL;
	}

	p->link.type = type;
	p->network.receiver = receiver;
	p->network.sender = address;

	transportBedCurrentPacket = p;
	transportBedCurrentPacketLength = size;

	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_CREATED_BED_PACKET);

	return p;
}

static void* CreateJoinRequestPacket()
{
	application_join_request_packet* p = Network_CreateBedPacket(GATEWAY_ADDRESS, TYPE_APPLICATION_JOIN_REQUEST, sizeof(application_join_request_packet));

	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_CREATED_JOIN_REQUEST);

	return p->serialNumber;
}

bool Network_QueueBedPacket()
{
	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_SENT_BED_PACKET);
	return QueueLinkPacket((link_network_header*) transportBedCurrentPacket, transportBedCurrentPacketLength);
}
// End BED protocol

// RDD protocol

static void* CreateRddPacket(uint8_t receiver, uint8_t type, uint8_t size)
{
	if (Queue_IsFull(transportRddQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_UNABLE_TO_CREATE_RDD_PACKET);
		return NULL;
	}
	queue_element* qe = Queue_Head(transportRddQueue);

	qe->size = size;
	qe->object = MemoryManager_Allocate(qe->size);

	if (qe->object == NULL)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_UNABLE_TO_CREATE_RDD_PACKET);
		return NULL;
	}

	link_network_rdd_transport_header* p = qe->object;
	p->link.type = type;
	p->network.receiver = receiver;
	p->network.sender = address;
	p->transport.sequenceNumber = transportRddNextSequenceNumber++;

	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_CREATED_RDD_PACKET);

	return qe->object;
}

void* Network_CreateSensorDataPacket(uint8_t receiver, uint8_t sensor, uint8_t dataSize)
{
	application_sensor_data_packet* p = CreateRddPacket(receiver, TYPE_APPLICATION_SENSOR_DATA, sizeof(application_sensor_data_packet) + dataSize);
	if (p == NULL)
	{
		return NULL;
	}

	p->sensor = sensor;

	return p->data;
}

void Network_CreateSetResponsePacket(uint8_t receiver, property_status status)
{
	application_set_response_packet* p = CreateRddPacket(receiver, TYPE_APPLICATION_SET_PROPERTY_RESPONSE, sizeof(application_get_response_packet));

	p->status = status;
}

void* Network_CreateGetResponsePacket(uint8_t receiver, property_status status, uint8_t dataSize)
{
	application_get_response_packet* p = CreateRddPacket(receiver, TYPE_APPLICATION_GET_PROPERTY_RESPONSE, sizeof(application_get_response_packet) + dataSize);

	p->status = status;

	return p->data;
}

void Network_QueueRddPacket()
{
	Queue_AdvanceHead(transportRddQueue);
}

static void AcknowledgeTransportRddPacket(link_network_rdd_transport_header* packet)
{

}

// End RDD protocol

// Internal

static void InitiateSynchronization()
{
	while (linkState != LINK_STATE_IDLE)
		;

	linkState = LINK_STATE_UNSYNCHRONIZED;
}

/**
 * Called directly from the timer interrupt and so it is atomic.
 */
void Network_TimerEvent()
{
	RadioDriver_EnableReceiveMode();


	// a node must be synchronized before it can communicate on the network
	if (linkState == LINK_STATE_UNSYNCHRONIZED)
	{
		return;
	}

	//Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_TICK);

	// update the CCA threshold
	UpdateCca();


	// if state is not STATE_IDLE a frame was not received when one was expected
	switch (linkState)
	{
		case LINK_STATE_EXPECTING_RTS:
			// CCA indicated a transmission but no packet arrived. Note noisy environment.
			break;

		case LINK_STATE_EXPECTING_CTS:
			// RTS was never replied to. Inform router that the node addressed is bad. Maybe a weak link
			if (++linkTransmissionCount >= NETWORK_LINK_MAXIMUM_TRANSMISSION_ATTEMPTS)
			{
				linkTransmissionCount = 0;
				if (Queue_IsEmpty(linkQueue) == false)
				{
					queue_element* qe = Queue_Tail(linkQueue);
					MemoryManager_Release(qe->object);
					Queue_AdvanceTail(linkQueue);
				}
			}
			break;

		case LINK_STATE_EXPECTING_DATA:
			// Got RTS but no Data. Inform router of weak link.
			break;

		case LINK_STATE_EXPECTING_ACK:
			// Data was never acknowledged. Inform router of weak link.
			if (++linkTransmissionCount >= NETWORK_LINK_MAXIMUM_TRANSMISSION_ATTEMPTS)
			{
				linkTransmissionCount = 0;
				if (Queue_IsEmpty(linkQueue) == false)
				{
					queue_element* qe = Queue_Tail(linkQueue);
					MemoryManager_Release(qe->object);
					Queue_AdvanceTail(linkQueue);
				}
			}
			break;
	}

	// reset state
	linkState = LINK_STATE_IDLE;

	uint8_t currentSlot = 0;

	if (Queue_IsEmpty(linkQueue) == false) // frames to send?
	{
		//uint8_t slot = rand_r(&randomContext) & 0x3; // if this is a high priority node or packet choose an earlier slot
		uint8_t slot = 0;
		for (; currentSlot < slot; currentSlot++)
		{
			if (IsChannelClear() == false)
			{
				break;
			}
			_delay_us(NETWORK_LINK_RTS_SLOT_DURATION);
		}

		if (IsChannelClear())
		{
			// get next packet and find the next node in its route
			queue_element* qe = Queue_Tail(linkQueue);
			linkCurrentPacket = qe->object;
			linkCurrentPacketLength = qe->size;


			// TODO: Extract destination and link loss for destination and calculate needed TX power level and set it

			// Send RTS
			rtsPacketTemplate.link.destination = linkCurrentPacket->link.destination;
			rtsPacketTemplate.slot = slot;
			rtsPacketTemplate.transmissionPowerLevel = 0; // TODO Implement this
			RadioDriver_Send(&rtsPacketTemplate, sizeof(rtsPacketTemplate));

			Diagnostics_SendEvent(DIAGNOSTICS_TX_RTS);

			linkState = LINK_STATE_EXPECTING_CTS;
		}
	}

	if (linkState == LINK_STATE_IDLE) // nothing to send => receive maybe
	{
		for (uint8_t i = currentSlot; i < NETWORK_LINK_NUMBER_OF_RTS_SLOTS; i++)
		{
			if (IsChannelClear() == false) // transmission in progress
			{
				linkState = LINK_STATE_EXPECTING_RTS;
				break;
			}
			_delay_us(NETWORK_LINK_RTS_SLOT_DURATION);
		}

		if (linkState == LINK_STATE_IDLE) // channel clear => no one is sending
		{
			RadioDriver_DisableReceiveMode();
		}
	}


	// transport timer
	if (transportRddTimer > 0)
	{
		if (--transportRddTimer == 0)
		{
			EventDispatcher_Complete(TransportRddTimeoutHandler);
		}
	}


	// process low resolution, high precision timers - high jitter if placed after network logic
}

static void TransportRddTimeoutHandler()
{
	transportRddTimer = NETWORK_TRANSPORT_RDD_TIMEOUT;
}

#if MASTER_NODE == 0
static void SynchronizationHandler()
{
	if (connected == false)
	{
		application_join_request_packet* p = CreateJoinRequestPacket();


		//NonVolatileStorage_Read(0, p->serialNumber, lengthof(p->serialNumber));

		p->serialNumber[0] = serialNumber;

		memset(p->serialNumber + 1, 0, 15);
		Network_QueueBedPacket();
		Leds_GreenOn();
	}
}
#endif

#if MASTER_NODE == 1
static void JoinRequestHandler(void* data, uint8_t length)
{
	static uint8_t nextAddress = 1;

	application_join_request_packet* p = data;

	application_join_response_packet* response = Network_CreateBedPacket(BROADCAST_ADDRESS, TYPE_APPLICATION_JOIN_RESPONSE, sizeof(application_join_response_packet));
	response->assignedAddress = nextAddress++;
	memcpy(response->serialNumber, p->serialNumber, lengthof(p->serialNumber));
	Network_QueueBedPacket();

	Leds_GreenOn();
}
#endif

#if MASTER_NODE == 0
static void JoinResponseHandler(void* data, uint8_t length)
{
	application_join_response_packet* p = data;


	//NonVolatileStorage_Read(0, p->serialNumber, lengthof(p->serialNumber));

	if (p->serialNumber[0] == serialNumber)
	{
		AssignAddress(p->assignedAddress);
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_JOINED);
		Leds_YellowOn();
	}
}
#endif

static void* FrameReceived(void* data, uint8_t length)
{
	link_header* lh = data;


#if MASTER_NODE == 0
	if (linkState == LINK_STATE_UNSYNCHRONIZED)
	{
		if (lh->type == TYPE_LINK_RTS)
		{
			//link_rts_packet* p = data;
			//uint8_t slot = p->slot;
			// set timer
			NetworkTimer_SetTimerValue(2);

			linkState = LINK_STATE_EXPECTING_RTS;

			EventDispatcher_Complete(SynchronizationHandler);
		}
		else
		{
			return data;
		}
	}
#endif

	if (lh->destination != address && lh->destination != BROADCAST_ADDRESS)
	{
		return data;
	}

	switch (lh->type)
	{
		// link layer packets

		case TYPE_LINK_RTS:
			{
				if (linkState != LINK_STATE_EXPECTING_RTS)
				{
					linkState = LINK_STATE_IDLE;
					break;
				}

				linkCurrentSource = lh->source;

				ctsPacketTemplate.link.destination = linkCurrentSource;
				RadioDriver_Send(&ctsPacketTemplate, sizeof(ctsPacketTemplate));

				linkState = LINK_STATE_EXPECTING_DATA;

				Diagnostics_SendEvent(DIAGNOSTICS_RX_RTS);
				Diagnostics_SendEvent(DIAGNOSTICS_TX_CTS);
			}
			break;

		case TYPE_LINK_CTS:
			{
				if (linkState != LINK_STATE_EXPECTING_CTS || linkCurrentPacket->link.destination != lh->source)
				{
					linkState = LINK_STATE_IDLE;
					break;
				}

				RadioDriver_Send(linkCurrentPacket, linkCurrentPacketLength);

				linkState = LINK_STATE_EXPECTING_ACK;

				Diagnostics_SendEvent(DIAGNOSTICS_RX_CTS);
				Diagnostics_SendEvent(DIAGNOSTICS_TX_DATA);
			}
			break;

		case TYPE_LINK_ACK:
			{
				if (linkState != LINK_STATE_EXPECTING_ACK || linkCurrentPacket->link.destination != lh->source)
				{
					linkState = LINK_STATE_IDLE;
					break;
				}

				MemoryManager_Release(linkCurrentPacket); // release allocated memory
				Queue_AdvanceTail(linkQueue); // and remove it from the queue

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

		case TYPE_TRANSPORT_RDD_ACK:
			{
				if (VerifyNetworkLayerHeader(&data, length))
				{
					queue_element* qe = Queue_Head(transportRddQueue);
					transport_rdd_acknowledge_packet* qp = qe->object;
					transport_rdd_acknowledge_packet* p = data;
					if (p->network.sender != qp->network.receiver || p->transport.sequenceNumber != qp->transport.sequenceNumber)
					{
						break;
					}
					// TODO: Send notification on timeout (acks just happens without a sound)
					Queue_AdvanceTail(transportRddQueue);
				}
			}
			break;


			// application layer packets
#if MASTER_NODE == 1
			case TYPE_APPLICATION_JOIN_REQUEST:
			if (ProcessBedPacket(&data, length, JoinRequestHandler))
			{
				//Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			}
			break;
#endif


#if MASTER_NODE == 0
		case TYPE_APPLICATION_JOIN_RESPONSE:
			if (ProcessBedPacket(&data, length, JoinResponseHandler))
			{
				//Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			}
			break;
#endif


			//		case TYPE_APPLICATION_SENSOR_DATA:
			//			if (ProcessNetworkPacket(&data, length, sensorDataProcessor))
			//			{
			//				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			//			}
			//			break;

		case TYPE_APPLICATION_SET_PROPERTY_REQUEST:
			if (ProcessRddPacket(&data, length, SensorManager_SetProperty))
			{
				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			}
			break;

		case TYPE_APPLICATION_GET_PROPERTY_REQUEST:
			if (ProcessRddPacket(&data, length, SensorManager_GetProperty))
			{
				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			}
			break;

		default:
			linkState = LINK_STATE_IDLE;
			break;
	}

	return data;
}

static bool VerifyNetworkLayerHeader(void** data, uint8_t length)
{
	link_header* lh = *data;
	link_network_header* lnh = *data;


	// link layer functionality

	// verify that a packet is expected and that the source is the node that sent the RTS
	if (linkState != LINK_STATE_EXPECTING_DATA || linkCurrentSource != lh->source)
	{
		linkState = LINK_STATE_IDLE;
		return false;
	}
	linkState = LINK_STATE_IDLE;


	// send link layer acknowledge
	ackPacketTemplate.link.destination = linkCurrentSource;
	RadioDriver_Send(&ackPacketTemplate, sizeof(ackPacketTemplate));


	// network layer functionality

	// forward if packet was not for this node
	if (lnh->network.receiver != address)
	{
		if (QueueLinkPacket(*data, length))
		{
			*data = NULL; // keep the allocated block
			Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_FORWARDING);
		}
		else
		{
			Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_FORWARDING_DROPPED);
		}

		return false;
	}

	return true;
}

static bool ProcessBedPacket(void** data, uint8_t length, block_handler packetHandler)
{
	if (VerifyNetworkLayerHeader(data, length)) // a network layer packet for this node?
	{
		if (EventDispatcher_Process(packetHandler, *data, length)) // add to event queue
		{
			*data = NULL; // keep the allocated memory

			return true;
		}
	}

	return false;
}

static bool ProcessRddPacket(void** data, uint8_t length, block_handler packetHandler)
{
	if (VerifyNetworkLayerHeader(data, length)) // a network layer packet for this node?
	{
		if (EventDispatcher_Process(packetHandler, *data, length)) // add to event queue
		{
			AcknowledgeTransportRddPacket(*data); // acknowledge packet if it could be added to queue

			*data = NULL; // keep the allocated memory

			return true;
		}
	}

	return false;
}

//
// Routing functionality

#define COST_INFINITY																								0xffff

static uint8_t PickCheapestNode(node nodes[])
{
	uint16_t lowestCost = COST_INFINITY;
	uint8_t index = NO_ROUTE;

	for (uint8_t i = 0; i < 15; i++)
	{
		if (nodes[i].marked == false)
		{
			if (nodes[i].cost < lowestCost)
			{
				index = i;
			}
		}
	}

	return index;
}

static bool FindRouteToNode(uint8_t target)
{
	node nodes[15];
	for (uint8_t i = 0; i < 15; i++)
	{
		node* n = &nodes[i];
		n->marked = false;
		n->cost = COST_INFINITY;
		n->previousNode = NO_ROUTE;
	}
	nodes[address].cost = 0;

	for (uint8_t i = 0; i < 15; i++)
	{
		uint8_t currentIndex = PickCheapestNode(nodes);
		if (currentIndex == NO_ROUTE)
		{
			routingTable[target] = NO_ROUTE;
			return false;
		}
		if (currentIndex == target)
		{
			uint8_t next = target;
			uint8_t previous = nodes[next].previousNode;
			while (previous != address)
			{
				next = previous;
				previous = nodes[next].previousNode;
			}
			routingTable[target] = next;
			return true;
		}

		node* current = &nodes[currentIndex];
		for (uint8_t neighborIndex = 0; neighborIndex < 15; neighborIndex++)
		{
			if (current->costToNeighbors[neighborIndex] != 0)
			{
				node* neighbor = &nodes[neighborIndex];
				uint16_t newCost = current->cost + current->costToNeighbors[neighborIndex];
				if (newCost < neighbor->cost)
				{
					neighbor->cost = newCost;
					neighbor->previousNode = currentIndex;
				}
			}
		}

		current->marked = true;
	}
}

//
// Clear Channel Assessment stuff here

#define RSSI_SAMPLE_COUNT																						8
#define CCA_ALPHA																										0.06
#define RSSI_OUTLIER_COUNT																					5
#define CCA_RSSI_CHECK_INTERVAL																			2

static int8_t ccaThreshold;

// TODO Please! use fixed point brrr
static void UpdateCca()
{
	static float rssiSamples[RSSI_SAMPLE_COUNT];
	static uint8_t rssiIndex = 0;
	static float oldMedian = 0;
	static float oldThreshold = 0;
	static float sum = 0;

	float rssi = RadioDriver_GetRssi();

	sum -= rssiSamples[rssiIndex];
	rssiSamples[rssiIndex] = rssi;
	sum += rssi;

	if (++rssiIndex >= RSSI_SAMPLE_COUNT)
	{
		rssiIndex = 0;
	}

	float median = sum / RSSI_SAMPLE_COUNT;

	oldThreshold = CCA_ALPHA * oldMedian + (1.0 - CCA_ALPHA) * oldThreshold;

	oldMedian = median;

	ccaThreshold = oldThreshold;
}

static bool IsChannelClear()
{
	uint8_t outliers = 0;

	for (uint8_t i = 0; i < RSSI_OUTLIER_COUNT; i++)
	{
		if (RadioDriver_GetRssi() <= ccaThreshold)
		{
			outliers++;
		}
		_delay_us(CCA_RSSI_CHECK_INTERVAL);
	}

	return outliers > 0;
}
