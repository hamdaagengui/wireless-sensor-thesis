/*
 * Network.c
 *
 *  Created on: 10/09/2010
 *      Author: Coma
 */

#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "Network.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../Collections/Queue.h"
#include "../HardwareAbstractionLayer/HardwareAbstractionLayer.h"
#include "../MemorySubsystem/MemoryManager.h"

//  Constants
#define RTS_DELAY_SLOT_DURATION																			100
#define BROADCAST_ID																								15

enum
{
	STATE_UNSYNCHRONIZED,
	STATE_IDLE,
	STATE_EXPECTING_CTS,
	STATE_EXPECTING_ACK,
	STATE_EXPECTING_DATA,
	STATE_EXPECTING_PACKET
};

typedef struct
{
	void* object;
	uint8_t size;
} linkPacketQueueElement;

static volatile uint8_t state;
static uint8_t currentLink;
static linkPacketQueueElement* currentPacket;
uint8_t linkPacketQueue[Queue_CalculateSize(sizeof(linkPacketQueueElement), NETWORK_LINK_PACKET_POOL_SIZE)];

typedef struct
{
	uint8_t destination :4; // set by the network layer (routing)
	uint8_t source :4;
	uint8_t type;
} link_header;

typedef struct
{
	uint8_t receiver :4;
	uint8_t sender :4;
} network_header;

typedef struct
{
	link_header link;
	network_header network;
} link_network_header;

enum
{
	TYPE_RTS,
	TYPE_CTS,
	TYPE_ACK,
	TYPE_DATA,
	TYPE_SENSOR_DATA,
	TYPE_SET,
	TYPE_SET_COMPLETE,
	TYPE_GET,
	TYPE_GET_COMPLETE,
	TYPE_READ,
	TYPE_READ_COMPLETE,
	TYPE_WRITE,
	TYPE_WRITE_COMPLETE
};

typedef struct
{
	link_header link;
	uint8_t slot;
} rts_packet;

typedef struct
{
	link_header link;
} cts_packet;

typedef struct
{
	link_header link;
} acknowledge_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t length;
	uint8_t data[];
} data_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t sensorId;
	uint8_t length;
	uint8_t data[];
} sensor_data_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t sensorId;
	uint8_t propertyId;
	uint8_t length;
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
	uint8_t sensorId;
	uint8_t propertyId;
} get_packet;

typedef struct
{
	link_header link;
	network_header network;
	uint8_t status;
	uint8_t length;
	uint8_t data[];
} get_complete_packet;

typedef struct
{
	link_header link;
	network_header network;
	char fieldName[10];
	uint8_t length;
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
	uint8_t length;
	uint8_t data[];
} read_complete_packet;

static uint8_t assignedNodeId;

static unsigned long randomContext;

static rts_packet rtsPacketTemplate;
static cts_packet ctsPacketTemplate;
static acknowledge_packet ackPacketTemplate;

static block_handler dataHandler;

static void ForwardPacket(link_network_header* packet, uint8_t length);
static void ConnectionEstablished();
static void FrameReceived(void* data, uint8_t length);
static uint8_t FindNext(uint8_t destination);
static void UpdateCCA();
static bool IsChannelClear();
static void InitiateSynchronization();

static bool isMaster;

void Network_Initialize(bool master)
{
	isMaster = master;

	Queue_Initialize(linkPacketQueue, sizeof(linkPacketQueueElement), NETWORK_LINK_PACKET_POOL_SIZE);

	RadioDriver_Initialize(FrameReceived);

	srand(RadioDriver_GetRandomNumber());


	//#ifdef NETWORK_MASTER_NODE
	if (isMaster)
	{
		ConnectionEstablished(); // uses default id (0) and does not need synchronization
		state = STATE_IDLE;
	}
	else
	{
		//#else
		InitiateSynchronization();
	}
	//#endif

	NetworkTimer_Initialize(0);
	NetworkTimer_SetTimerPeriod(256);
	NetworkTimer_SetTimerValue(0);
}

void Network_SetId(uint8_t id)
{
	assignedNodeId = id;
	ConnectionEstablished();
}

void Network_SetDataHandler(block_handler handler)
{
	dataHandler = handler;
}

bool Network_SendData(uint8_t receiver, void* data, uint8_t length)
{
	Critical();

	if (Queue_IsFull(linkPacketQueue))
	{
		NonCritical();
		return false;
	}

	data_packet* p = MemoryManager_AllocateNetworkBlock();

	if (p == NULL)
	{
		// no free network buffer objects
		NonCritical();
		return false;
	}

	p->link.source = assignedNodeId;
	p->link.type = TYPE_DATA;
	p->network.receiver = receiver;
	p->network.sender = assignedNodeId;
	p->length = length;
	memcpy(p->data, data, length);

	linkPacketQueueElement* e = Queue_Head(linkPacketQueue);
	e->object = p;
	e->size = sizeof(data_packet) + length;
	Queue_AdvanceHead(linkPacketQueue);

	NonCritical();
	return true;
}

//bool Network_SendSensorData(uint8_t receiver, uint8_t sensorId, void* data, uint8_t length)
//{
//	sensor_data_packet* p = MemoryManager_AllocateNetworkBlock();
//	if (p == NULL)
//	{
//		// no free network buffer objects
//		return false;
//	}
//
//	p->link.source = assignedNodeId;
//	p->link.type = TYPE_SENSOR_DATA;
//	p->network.receiver = receiver;
//	p->network.sender = assignedNodeId;
//	p->sensorId = sensorId;
//	p->length = length;
//	memcpy(p->data, data, length);
//
//	return true;
//}

// Internal

static void ConnectionEstablished()
{
	rtsPacketTemplate.link.source = assignedNodeId;
	rtsPacketTemplate.link.type = TYPE_RTS;
	ctsPacketTemplate.link.source = assignedNodeId;
	ctsPacketTemplate.link.type = TYPE_CTS;
	ackPacketTemplate.link.source = assignedNodeId;
	ackPacketTemplate.link.type = TYPE_ACK;
}

static void InitiateSynchronization()
{
	while (state != STATE_IDLE)
		;

	state = STATE_UNSYNCHRONIZED;
}

/**
 * Called directly from the timer interrupt and so it atomic.
 */
void Network_TimerEvent()
{
	Leds_GreenToggle();

	if (state == STATE_UNSYNCHRONIZED)
	{
		return;
	}

	UpdateCCA();

	switch (state)
	{
		case STATE_EXPECTING_CTS:
			// RTS was never replied to. Inform router that the node addressed is bad. Maybe a weak link
			break;

		case STATE_EXPECTING_ACK:
			// Data was never acknowledged. Inform router of weak link.
			break;

		case STATE_EXPECTING_DATA:
			// Got RTS but no Data. Inform router of weak link.
			break;

		case STATE_EXPECTING_PACKET:
			// CCA indicated a transmission but no packet arrived. Note noisy environment.
			break;
	}

	state = STATE_IDLE;

	if (Queue_IsEmpty(linkPacketQueue) == false) // frames to send
	{
		uint8_t slot = rand_r(&randomContext) & 0x3; // if this is a high priority node or packet choose an earlier slot
		for (uint8_t i = 0; i < slot; i++)
		{
			if (IsChannelClear() == false)
			{
				break;
			}
			_delay_us(RTS_DELAY_SLOT_DURATION);
		}

		Leds_RedToggle();

		if (IsChannelClear())
		{
			currentPacket = Queue_Tail(linkPacketQueue);
			link_network_header* lnh = currentPacket->object;
			currentLink = FindNext(lnh->network.receiver);
			lnh->link.destination = currentLink;

			if (IsChannelClear())
			{
				rtsPacketTemplate.link.destination = currentLink;
				rtsPacketTemplate.slot = slot;
				RadioDriver_Send(&rtsPacketTemplate, sizeof(rtsPacketTemplate));

				state = STATE_EXPECTING_CTS;
			}
		}

		if (state != STATE_EXPECTING_CTS) // receive the packet that's coming
		{
			state = STATE_EXPECTING_PACKET;
			//RadioDriver_EnableReceiveMode();
		}
	}
	else // nothing to send => receive
	{
		state = STATE_EXPECTING_PACKET;
		//RadioDriver_EnableReceiveMode();
		//		for (uint8_t i = 0; i < 10; i++)
		//		{
		//			if (IsChannelClear() == false)
		//			{
		//				state = STATE_EXPECTING_PACKET;
		//				RadioDriver_EnableReceiveMode();
		//				break;
		//			}
		//			_delay_us(RTS_DELAY_SLOT_DURATION);
		//		}
	}

	static uint8_t timer;
	static uint8_t ticker;

	if (++timer >= 128)
	{
		timer++;
		Network_SendData(1, &ticker, 1);
		timer = 0;
	}


	// process low resolution, high precision timers
}

static void FrameReceived(void* data, uint8_t length)
{
	Leds_YellowToggle();

	link_header* lh = data;
	link_network_header* lnh = data;

	if (lh->destination == assignedNodeId || lh->destination == BROADCAST_ID)
	{
		switch (lh->type)
		{
			// Non broadcast and non routable
			case TYPE_RTS: // RTS is never broadcasted
				{
					if (state == STATE_UNSYNCHRONIZED) // TODO Not for MasterNode
					{
						rts_packet* p = data;
						uint8_t slot = p->slot;
						// set timer
						NetworkTimer_SetTimerValue(20);
					}

					if (state == STATE_EXPECTING_PACKET || state == STATE_UNSYNCHRONIZED)
					{
						ctsPacketTemplate.link.destination = lh->source;
						RadioDriver_Send(&ctsPacketTemplate, sizeof(ctsPacketTemplate));

						currentLink = lh->source;

						state = STATE_EXPECTING_DATA;
					}

					MemoryManager_ReleaseAnyBlock(data);
				}
				break;

			case TYPE_CTS: // CTS is never broadcasted
				{
					if (state == STATE_EXPECTING_CTS && currentLink == lh->source)
					{
						RadioDriver_Send(currentPacket->object, currentPacket->size);

						state = STATE_EXPECTING_ACK;
					}
					else
					{
						state = STATE_IDLE;
					}

					MemoryManager_ReleaseAnyBlock(data);
				}
				break;

			case TYPE_ACK:
				{
					if (state == STATE_EXPECTING_ACK && currentLink == lh->source)
					{
						Queue_AdvanceTail(linkPacketQueue); // remove it from the queue
						MemoryManager_ReleaseAnyBlock(currentPacket);
					}

					MemoryManager_ReleaseAnyBlock(data);

					state = STATE_IDLE;
				}
				break;


				// Routable packets

			case TYPE_DATA:
				{
					if (state == STATE_EXPECTING_DATA && currentLink == lh->source)
					{
						ackPacketTemplate.link.destination = lh->source;
						RadioDriver_Send(&ackPacketTemplate, sizeof(ackPacketTemplate));

						if (lnh->network.receiver == assignedNodeId)
						{
							// data_packet* p = data;
							// pass on data up the stack
							//		EventDispatcher_Process(dataHandler, )

							// dummy release since its not handled here yet
							MemoryManager_ReleaseAnyBlock(data);
						}
						else // forward data
						{
							lnh->link.source = assignedNodeId; // change source to indicate that we are the new link layer source
							ForwardPacket(data, length);
						}
					}
					else
					{
						MemoryManager_ReleaseAnyBlock(data);
					}

					state = STATE_IDLE;
				}
				break;
		}
	}
}

//

static void ForwardPacket(link_network_header* packet, uint8_t length)
{
	//	Critical();
	//
	//	if (Queue_IsFull(linkPacketQueue))
	//	{
	//		NonCritical();
	//		return;
	//	}
	//
	//	data_packet* p = MemoryManager_AllocateNetworkBlock();
	//
	//	if (p == NULL)
	//	{
	//		// no free network buffer objects
	//		NonCritical();
	//		return;
	//	}
	//
	//	p->link.source = assignedNodeId;
	//	p->link.type = TYPE_DATA;
	//	p->network.receiver = receiver;
	//	p->network.sender = assignedNodeId;
	//	p->length = length;
	//	memcpy(p->data, data, length);
	//
	//	linkPacketQueueElement* e = Queue_Head(linkPacketQueue);
	//	e->object = p;
	//	e->size = sizeof(data_packet) + length;
	//	Queue_AdvanceHead(linkPacketQueue);
	//
	//	NonCritical();
	return;
}

// Routing functionality

static uint8_t FindNext(uint8_t destination)
{
	return destination;
}

static void LinkTransmissionTimeout()
{
	// routeinfo.link[currentlink].timeoutcounter++
}

#define RSSI_SAMPLE_COUNT																						8
#define CCA_ALPHA																										0.06
#define RSSI_OUTLIER_COUNT																					5
#define CCA_RSSI_CHECK_INTERVAL																			2

// TODO Please! use fixed point brrr
static int8_t ccaThreshold = -75;

static void UpdateCCA()
{
	return;
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
	for (uint8_t i = 0; i < RSSI_OUTLIER_COUNT; i++)
	{
		if (RadioDriver_GetRssi() < ccaThreshold)
		{
			return true;
		}

		_delay_us(CCA_RSSI_CHECK_INTERVAL);
	}

	return false;
}
