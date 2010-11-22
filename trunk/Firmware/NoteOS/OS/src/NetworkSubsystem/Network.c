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

//  Constants
#define BROADCAST_ADDRESS																						15

enum
{
	LINK_STATE_UNSYNCHRONIZED,
	LINK_STATE_IDLE,
	LINK_STATE_EXPECTING_RTS,
	LINK_STATE_EXPECTING_CTS,
	LINK_STATE_EXPECTING_ACK,
	LINK_STATE_EXPECTING_DATA
};

// headers for the individual layers
typedef struct
{
	uint8_t destination :4; // set by the network layer (routing)
	uint8_t source :4;
	uint8_t :4;
	uint8_t type :4;
} link_header;

typedef struct
{
	uint8_t receiver :4;
	uint8_t sender :4;
} network_header;

typedef struct
{
	uint8_t sequenceNumber;
} transport_header;

// combined headers
typedef struct
{
	link_header link;
	network_header network;
} link_network_header;

typedef struct
{
	link_header link;
	network_header network;
	transport_header transport;
	uint8_t payload[];
} link_network_transport_header;

// possible packet types
enum
{
	// link packets
	TYPE_LINK_RTS,
	TYPE_LINK_CTS,
	TYPE_LINK_ACK,
	// network packets
	TYPE_NETWORK_ROUTES,
	// transport packets
	TYPE_TRANSPORT_ACK,
	// application packets
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
};

typedef struct
{
	link_header link;
	uint8_t slot;
	int8_t transmissionPowerLevel;
	uint8_t energyLevel;
} link_rts_packet;

typedef struct
{
	link_header link;
} link_cts_packet;

typedef struct
{
	link_header link;
} link_acknowledge_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t distances[15];
} network_routes_packet;

typedef struct
{
	link_header link;
	network_header network;
	transport_header transport;
} transport_acknowledge_packet;

typedef struct
{
	link_header link;
	network_header network;
	transport_header transport;
	uint8_t sensor;
	uint8_t data[];
} application_sensor_data_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t sensor;
	uint8_t propertyId;
	uint8_t data[];
} set_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t status;
} set_complete_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t sensor;
	uint8_t propertyId;
} get_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t status;
	uint8_t data[];
} get_complete_packet;

typedef struct
{
	link_header link;
	network_header network;
	char fieldName[10];
	uint8_t data[];
} write_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t status;
} write_complete_packet;

typedef struct
{
	link_header link;
	network_header network;
	char fieldName[10];
} read_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t status;
	uint8_t data[];
} read_complete_packet;

static void PreparePreloadedPackets();
static void TransportTimeoutHandler();
static void* FrameReceived(void* data, uint8_t length);
static bool VerifyNetworkLayerHeader(void** data, uint8_t length);
static bool ProcessNetworkPacket(void** data, uint8_t length, block_handler packetHandler);
static uint8_t FindNextNodeInRoute(uint8_t destination);
static void UpdateCca();
static bool IsChannelClear();
static void InitiateSynchronization();

// misc
//static unsigned long randomContext;
static block_handler sensorDataProcessor;

// link
static volatile uint8_t linkState;
static uint8_t currentSource;
static link_network_header* currentLinkPacket;
static uint8_t currentLinkPacketLength;
static uint8_t linkQueue[Queue_CalculateSize(sizeof(queue_element), NETWORK_LINK_QUEUE_SIZE)];

static link_rts_packet rtsPacketTemplate;
static link_cts_packet ctsPacketTemplate;
static link_acknowledge_packet ackPacketTemplate;

static uint8_t address;

// network
static uint8_t distances[15][15];

// transport
static uint8_t nextSequenceNumber;
static uint8_t transportQueue[Queue_CalculateSize(sizeof(queue_element), NETWORK_TRANSPORT_QUEUE_SIZE)];
static link_network_transport_header* currentTransportPacket;
static uint8_t currentTransportPacketLength;
static uint8_t transportTimer;

void Network_Initialize()
{
	Queue_Initialize(linkQueue, sizeof(queue_element), NETWORK_LINK_QUEUE_SIZE);
	Queue_Initialize(transportQueue, sizeof(queue_element), NETWORK_TRANSPORT_QUEUE_SIZE);

	RadioDriver_SetFrameReceivedHandler(FrameReceived);

	srand(RadioDriver_GetRandomNumber());


#ifdef MASTER_NODE
	PreparePreloadedPackets(); // uses default id (0) and does not need synchronization
	linkState = LINK_STATE_IDLE;
#else
	linkState = LINK_STATE_UNSYNCHRONIZED;
#endif

	NetworkTimer_Initialize();
	NetworkTimer_SetTimerPeriod(NETWORK_TIMER_FREQUENCY / NETWORK_COMMUNICATION_SLOT_FREQUENCY);
	NetworkTimer_SetTimerValue(0);
}

void Network_SetAddress(uint8_t adr)
{
	address = adr;
	PreparePreloadedPackets();
}

void Network_Handlers(block_handler sensorDataHandler)
{
	sensorDataProcessor = sensorDataHandler;
}

void Network_SendPacket()
{
	Queue_AdvanceHead(transportQueue);
}

void* Network_CreateTransportPacket(uint8_t receiver, uint8_t type, uint8_t size)
{
	if (Queue_IsFull(transportQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_DATA_NOT_QUEUED);
		return NULL;
	}
	queue_element* qe = Queue_Head(transportQueue);

	qe->size = size;
	qe->object = MemoryManager_Allocate(qe->size);

	if (qe->object == NULL)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_DATA_NOT_QUEUED);
		return NULL;
	}

	link_network_transport_header* p = qe->object;
	p->link.source = address;
	p->link.type = type;
	p->network.receiver = receiver;
	p->network.sender = address;
	p->transport.sequenceNumber = nextSequenceNumber++;

	return qe->object;
}

void* Network_CreateSensorDataPacket(uint8_t receiver, uint8_t sensor, uint8_t dataSize)
{
	application_sensor_data_packet* p = Network_CreateTransportPacket(receiver, TYPE_APPLICATION_SENSOR_DATA, sizeof(application_sensor_data_packet) + dataSize);

	p->sensor = sensor;

	return p->data;
}

// Internal

static void PreparePreloadedPackets()
{
	rtsPacketTemplate.link.source = address;
	rtsPacketTemplate.link.type = TYPE_LINK_RTS;
	ctsPacketTemplate.link.source = address;
	ctsPacketTemplate.link.type = TYPE_LINK_CTS;
	ackPacketTemplate.link.source = address;
	ackPacketTemplate.link.type = TYPE_LINK_ACK;
}

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
			break;

		case LINK_STATE_EXPECTING_ACK:
			// Data was never acknowledged. Inform router of weak link.
			break;

		case LINK_STATE_EXPECTING_DATA:
			// Got RTS but no Data. Inform router of weak link.
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
			_delay_us(NETWORK_RTS_SLOT_DURATION);
		}

		if (IsChannelClear())
		{
			// get next packet and find the next node in its route
			queue_element* qe = Queue_Tail(linkQueue);
			currentLinkPacket = qe->object;
			currentLinkPacketLength = qe->size;
			currentLinkPacket->link.destination = FindNextNodeInRoute(currentLinkPacket->network.receiver);


			// Send RTS
			rtsPacketTemplate.link.destination = currentLinkPacket->link.destination;
			rtsPacketTemplate.slot = slot;
			rtsPacketTemplate.transmissionPowerLevel = 0; // TODO Implement this
			rtsPacketTemplate.energyLevel = 0; // TODO Implement this
			RadioDriver_Send(&rtsPacketTemplate, sizeof(rtsPacketTemplate));

			Diagnostics_SendEvent(DIAGNOSTICS_TX_RTS);

			linkState = LINK_STATE_EXPECTING_CTS;
		}
	}

	if (linkState == LINK_STATE_IDLE) // nothing to send => receive maybe
	{
		for (uint8_t i = currentSlot; i < NETWORK_NUMBER_OF_RTS_SLOTS; i++)
		{
			if (IsChannelClear() == false) // transmission in progress
			{
				linkState = LINK_STATE_EXPECTING_RTS;
				break;
			}
			_delay_us(NETWORK_RTS_SLOT_DURATION);
		}

		if (linkState == LINK_STATE_IDLE) // channel clear => no one is sending
		{
			RadioDriver_DisableReceiveMode();
		}
	}


	// transport timer
	if (transportTimer > 0)
	{
		if (--transportTimer == 0)
		{
			EventDispatcher_Notify(TransportTimeoutHandler);
		}
	}


	// process low resolution, high precision timers - high jitter if placed after network logic
}

static void TransportTimeoutHandler()
{
	transportTimer = NETWORK_TRANSPORT_TIMEOUT;

}

static void AcknowledgeTransportPacket(link_network_transport_header* packet)
{
	transport_acknowledge_packet p;
	p.link;
}

static void* FrameReceived(void* data, uint8_t length)
{
	link_header* lh = data;
	link_network_header* lnh = data;


#ifndef MASTER_NODE
	if (linkState == LINK_STATE_UNSYNCHRONIZED)
	{
		if (lh->type == TYPE_LINK_RTS)
		{
			link_rts_packet* p = data;
			//uint8_t slot = p->slot;
			// set timer
			NetworkTimer_SetTimerValue(2);

			linkState = LINK_STATE_EXPECTING_RTS;
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

				currentSource = lh->source;

				ctsPacketTemplate.link.destination = currentSource;
				RadioDriver_Send(&ctsPacketTemplate, sizeof(ctsPacketTemplate));

				linkState = LINK_STATE_EXPECTING_DATA;

				Diagnostics_SendEvent(DIAGNOSTICS_RX_RTS);
			}
			break;

		case TYPE_LINK_CTS:
			{
				if (linkState != LINK_STATE_EXPECTING_CTS || currentLinkPacket->link.destination != lh->source)
				{
					linkState = LINK_STATE_IDLE;
					break;
				}

				RadioDriver_Send(currentLinkPacket, currentLinkPacketLength);

				linkState = LINK_STATE_EXPECTING_ACK;

				Diagnostics_SendEvent(DIAGNOSTICS_RX_CTS);
				Diagnostics_SendEvent(DIAGNOSTICS_TX_DATA);
			}
			break;

		case TYPE_LINK_ACK:
			{
				if (linkState != LINK_STATE_EXPECTING_ACK || currentLinkPacket->link.destination != lh->source)
				{
					linkState = LINK_STATE_IDLE;
					break;
				}

				MemoryManager_Release(currentLinkPacket); // release allocated memory
				Queue_AdvanceTail(linkQueue); // and remove it from the queue

				linkState = LINK_STATE_IDLE;

				Diagnostics_SendEvent(DIAGNOSTICS_RX_ACK);
			}
			break;


			// network layer packets

		case TYPE_NETWORK_ROUTES:
			{
				if (VerifyNetworkLayerHeader(&data, length)) // a network layer packet for this node?
				{
					network_routes_packet* p = data;
					memcpy(distances[p->network.sender], p->distances, 15);
				}
			}
			break;


			// transport layer packets

		case TYPE_TRANSPORT_ACK:
			{
				if (VerifyNetworkLayerHeader(&data, length)) // a network layer packet for this node?
				{
					queue_element* qe = Queue_Head(transportQueue);
					transport_acknowledge_packet* qp = qe->object;
					transport_acknowledge_packet* p = data;
					if (p->network.sender != qp->network.receiver || p->transport.sequenceNumber != qp->transport.sequenceNumber)
					{
						break;
					}
					Queue_AdvanceTail(transportQueue);
				}
			}
			break;


			// application layer packets

			//		case TYPE_APPLICATION_SENSOR_DATA:
			//			if (ProcessNetworkPacket(&data, length, sensorDataProcessor))
			//			{
			//				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			//			}
			//			break;
			//
			//		case TYPE_APPLICATION_SET_PROPERTY_REQUEST:
			//			if (ProcessNetworkPacket(&data, length, sensorSubSystemGetProperty))
			//			{
			//				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			//			}
			//			break;
			//
			//		case TYPE_APPLICATION_SET_PROPERTY_REQUEST:
			//			if (ProcessNetworkPacket(&data, length, sensorSubSystemGetProperty))
			//			{
			//				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			//			}
			//			break;
	}

	return data;
}

static bool VerifyNetworkLayerHeader(void** data, uint8_t length)
{
	link_header* lh = *data;
	link_network_header* lnh = *data;


	// link layer functionality

	// verify that a packet is expected and that the source is the node that sent the RTS
	if (linkState != LINK_STATE_EXPECTING_DATA || currentSource != lh->source)
	{
		linkState = LINK_STATE_IDLE;
		return false;
	}
	linkState = LINK_STATE_IDLE;


	// send link layer acknowledge
	ackPacketTemplate.link.destination = currentSource;
	RadioDriver_Send(&ackPacketTemplate, sizeof(ackPacketTemplate));


	// network layer functionality

	// forward if packet was not for this node
	if (lnh->network.receiver != address)
	{
		if (Queue_IsFull(linkQueue) == false)
		{
			lh->source = address;
			queue_element* qe = Queue_Head(linkQueue);
			qe->size = length;
			qe->object = *data;
			Queue_AdvanceHead(linkQueue);

			*data = NULL; // keep the allocated block

			Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_FORWARDING);
		}

		return false;
	}

	return true;
}

static bool ProcessNetworkPacket(void** data, uint8_t length, block_handler packetHandler)
{
	if (VerifyNetworkLayerHeader(data, length)) // a network layer packet for this node?
	{
		if (EventDispatcher_Process(packetHandler, *data, length)) // add to event queue
		{
			AcknowledgeTransportPacket(*data); // acknowledge packet if it could be added to queue

			*data = NULL; // keep the allocated memory

			return true;
		}
	}

	return false;
}

// Routing functionality

static uint8_t FindNextNodeInRoute(uint8_t destination)
{
	return destination;
}

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

/*
 01234567

 0 00000000
 1 10000000
 2 10000000
 3 00000000
 4 00000000
 5 00000000
 6 00000000
 7 00000000

 0 00000000
 1 00000000
 2 00000000
 3 00000000
 4 00000000
 5 00000000
 6 00000000
 7 00000000

 0 00000000
 1 00000000
 2 00000000
 3 00000000
 4 00000000
 5 00000000
 6 00000000
 7 00000000

 */
