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
#include "../HardwareAbstractionLayer/RadioDriver.h"
#include "../HardwareAbstractionLayer/NetworkTimer.h"
#include "../MemorySubsystem/MemoryManager.h"
#include "../Diagnostics/Diagnostics.h"
#include "../SensorSubsystem/SensorManager.h"

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

static void TransportTimeoutHandler();
static void* FrameReceived(void* data, uint8_t length);
static bool VerifyNetworkLayerHeader(void** data, uint8_t length);
static bool ProcessNetworkPacket(void** data, uint8_t length, block_handler packetHandler);
static void UpdateCca();
static bool IsChannelClear();
static void InitiateSynchronization();
static bool SendLinkPacket(link_network_header* packet, uint8_t length);

// misc
//static unsigned long randomContext;
static block_handler sensorDataProcessor = NULL;

// link
#ifdef MASTER_NODE

static volatile uint8_t linkState = LINK_STATE_IDLE;
static uint8_t address = 0;
static bool connected = true;
static link_rts_packet rtsPacketTemplate = { .link.source = 0, .link.type = TYPE_LINK_RTS };
static link_cts_packet ctsPacketTemplate = { .link.source = 0, .link.type = TYPE_LINK_CTS };
static link_acknowledge_packet ackPacketTemplate = { .link.source = 0, .link.type = TYPE_LINK_ACK };

#else

static volatile uint8_t linkState = LINK_STATE_UNSYNCHRONIZED;
static uint8_t address = BROADCAST_ADDRESS;
static bool connected = false;
static link_rts_packet rtsPacketTemplate =
{	.link.type=TYPE_LINK_RTS};
static link_cts_packet ctsPacketTemplate =
{	.link.type=TYPE_LINK_CTS};
static link_acknowledge_packet ackPacketTemplate =
{	.link.type=TYPE_LINK_ACK};

#endif

static uint8_t currentSource;
static link_network_header* currentLinkPacket;
static uint8_t currentLinkPacketLength;
static uint8_t linkQueue[Queue_CalculateSize(sizeof(queue_element), NETWORK_LINK_QUEUE_SIZE)];

// network
#define NO_ROUTE																										0xff
static uint8_t routingTable[15] = { NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE, NO_ROUTE };

#define NETWORK_UNREACHABLE																					0xffff
#define LINK_DEAD																										0
//typedef struct
//{
//	int8_t txPower; // current transmission power level of node
//	uint8_t hops[15]; // hops from node to indexed node
//	uint8_t linkLoss[15]; // link loss between node and indexed node
//	uint16_t cost; // cost from node to indexed node
//} routing_entry;
//static routing_entry routingTable_[15];

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
//   RDD
static uint8_t rddNextSequenceNumber = 0;
static uint8_t rddQueue[Queue_CalculateSize(sizeof(queue_element), NETWORK_RDD_QUEUE_SIZE)];
static link_network_rdd_transport_header* rddCurrentPacket;
static uint8_t rddCurrentPacketLength;
static uint8_t rddTimer;

void Network_Initialize()
{
	Queue_Initialize(linkQueue, sizeof(queue_element), NETWORK_LINK_QUEUE_SIZE);
	Queue_Initialize(rddQueue, sizeof(queue_element), NETWORK_RDD_QUEUE_SIZE);

	RadioDriver_SetFrameReceivedHandler(FrameReceived);

	srand(RadioDriver_GetRandomNumber());

	NetworkTimer_Initialize();
	NetworkTimer_SetTimerPeriod(NETWORK_TIMER_FREQUENCY / NETWORK_COMMUNICATION_SLOT_FREQUENCY);
	NetworkTimer_SetTimerValue(0);
}

void Network_AssignAddress(uint8_t adr)
{
	address = adr;
	rtsPacketTemplate.link.source = address;
	ctsPacketTemplate.link.source = address;
	ackPacketTemplate.link.source = address;
}

void Network_Handlers(block_handler sensorDataHandler)
{
	sensorDataProcessor = sensorDataHandler;
}

static void* Network_CreateRddTransportPacket(uint8_t receiver, uint8_t type, uint8_t size)
{
	if (Queue_IsFull(rddQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_DATA_NOT_QUEUED);
		return NULL;
	}
	queue_element* qe = Queue_Head(rddQueue);

	qe->size = size;
	qe->object = MemoryManager_Allocate(qe->size);

	if (qe->object == NULL)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_DATA_NOT_QUEUED);
		return NULL;
	}

	link_network_rdd_transport_header* p = qe->object;
	p->link.source = address;
	p->link.type = type;
	p->network.receiver = receiver;
	p->network.sender = address;
	p->transport.sequenceNumber = rddNextSequenceNumber++;

	return qe->object;
}

void* Network_CreateSensorDataPacket(uint8_t receiver, uint8_t sensor, uint8_t dataSize)
{
	application_sensor_data_packet* p = Network_CreateRddTransportPacket(receiver, TYPE_APPLICATION_SENSOR_DATA, sizeof(application_sensor_data_packet) + dataSize);

	p->sensor = sensor;

	return p->data;
}

void Network_CreateSetResponsePacket(uint8_t receiver, property_status status)
{
	application_set_response_packet* p = Network_CreateRddTransportPacket(receiver, TYPE_APPLICATION_SET_PROPERTY_RESPONSE, sizeof(application_get_response_packet));

	p->status = status;
}

void* Network_CreateGetResponsePacket(uint8_t receiver, property_status status, uint8_t dataSize)
{
	application_get_response_packet* p = Network_CreateRddTransportPacket(receiver, TYPE_APPLICATION_GET_PROPERTY_RESPONSE, sizeof(application_get_response_packet) + dataSize);

	p->status = status;

	return p->data;
}

void Network_SendRddPacket()
{
	Queue_AdvanceHead(rddQueue);
}

static void AcknowledgeTransportRddPacket(link_network_rdd_transport_header* packet)
{

}

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


			// TODO: This must be moved to the place where the packet is queued.
			currentLinkPacket->link.destination = routingTable[currentLinkPacket->network.receiver];


			// TODO: Extract destination and link loss for destination and calculate needed TX power level and set it

			// Send RTS
			rtsPacketTemplate.link.destination = currentLinkPacket->link.destination;
			rtsPacketTemplate.slot = slot;
			rtsPacketTemplate.transmissionPowerLevel = 0; // TODO Implement this
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
	if (rddTimer > 0)
	{
		if (--rddTimer == 0)
		{
			EventDispatcher_Notify(TransportTimeoutHandler);
		}
	}


	// process low resolution, high precision timers - high jitter if placed after network logic
}

static void TransportTimeoutHandler()
{
	rddTimer = NETWORK_TRANSPORT_TIMEOUT;

}

static void SynchronizationHandler()
{
	if (connected == false)
	{
		// TODO: Send join request
		application_join_request_packet p;
		p.link.type = TYPE_APPLICATION_JOIN_REQUEST;
		p.network.sender = BROADCAST_ADDRESS;
		p.network.receiver = GATEWAY_ADDRESS;
	}
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

			EventDispatcher_Notify(SynchronizationHandler);
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
				if (VerifyNetworkLayerHeader(&data, length))
				{
					network_routes_packet* p = data;
					//memcpy(distances[p->network.sender], p->distances, 15);
				}
			}
			break;


			// transport layer packets

		case TYPE_TRANSPORT_RDD_ACK:
			{
				if (VerifyNetworkLayerHeader(&data, length))
				{
					queue_element* qe = Queue_Head(rddQueue);
					transport_rdd_acknowledge_packet* qp = qe->object;
					transport_rdd_acknowledge_packet* p = data;
					if (p->network.sender != qp->network.receiver || p->transport.sequenceNumber != qp->transport.sequenceNumber)
					{
						break;
					}
					// TODO: Send notification on timeout (acks just happens without a sound)
					Queue_AdvanceTail(rddQueue);
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

		case TYPE_APPLICATION_SET_PROPERTY_REQUEST:
			if (ProcessNetworkPacket(&data, length, SensorManager_SetProperty))
			{
				Diagnostics_SendEvent(DIAGNOSTICS_RX_SENSOR_DATA);
			}
			break;

		case TYPE_APPLICATION_GET_PROPERTY_REQUEST:
			if (ProcessNetworkPacket(&data, length, SensorManager_GetProperty))
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

static bool SendLinkPacket(link_network_header* packet, uint8_t length)
{
	if (Queue_IsFull(linkQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_LINK_BUFFER_OVERRUN);
		return false;
	}

	packet->link->source = address;
	queue_element* qe = Queue_Head(linkQueue);
	qe->size = length;
	qe->object = packet;
	Queue_AdvanceHead(linkQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_NETWORK_LINK_PACKET_QUEUED);

	return false;
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
		if (SendLinkPacket(*data, length))
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

static bool ProcessNetworkPacket(void** data, uint8_t length, block_handler packetHandler)
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
