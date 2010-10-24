/*
 * Network.c
 *
 *  Created on: 10/09/2010
 *      Author: Coma
 */

#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "Network.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../Collections/FIFO.h"
#include "../HardwareAbstractionLayer/HardwareAbstractionLayer.h"

#define ROUTE_ENTRY_TIMEOUT																					100
#define QUEUE_APPLICATION																						0
#define QUEUE_NETWORK																								1
#define QUEUE_SIZE																									50

#define NETWORK_DATA_RATE																						31250
#define NETWORK_TRANSCEIVER_TX_OVERHEAD_TICKS												5 // time for transceiver to switch to tx mode
#define NETWORK_TRANSCEIVER_TX_OVERHEAD_BYTES												8 // bytes used for preample etc
#define NETWORK_TICKS_PER_SECOND																		32768
#define NETWORK_TICKS_PER_FRAME																			1024
#define NETWORK_FRAMES_PER_SECOND																		(NETWORK_CLOCK / NETWORK_TICKS_PER_FRAME)
#define NETWORK_INITIAL_DELAY																				100
#define NETWORK_MASTER_NODE_MAXIMUM_FRAME_SIZE											32
#define NETWORK_MASTER_NODE_MAXIMUM_BYTES_OVER_THE_AIR							(NETWORK_TRANSCEIVER_TX_OVERHEAD_BYTES + NETWORK_MASTER_NODE_MAXIMUM_FRAME_SIZE)
#define NETWORK_CALCULATE_TICKS(bytes)															(NETWORK_TRANSCEIVER_TX_OVERHEAD_TICKS + ((NETWORK_TICKS_PER_SECOND * bytes) / NETWORK_DATA_RATE) + 1)
#define NETWORK_MASTER_NODE_TIME_SLOT_LENGTH												NETWORK_CALCULATE_TICKS(NETWORK_MASTER_NODE_MAXIMUM_BYTES_OVER_THE_AIR)

// NEW VERSION
// 	Configuration
#define NETWORK_MAXIMUM_FRAME_SIZE																	80
//  Constants
#define RTS_DELAY_SLOT_DURATION																			100

#define BROADCAST_ID																								15

uint8_t frameQueue[FIFO_CalculateSize(MESSAGE_QUEUE_SIZE)];

typedef struct
{
	uint8_t destination :4;
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
	TYPE_WRITE_COMPLETE,
//	TYPE_,
//	TYPE_,
//	TYPE_,
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
	uint16_t address;
	uint8_t length;
	//char fieldName[10]
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
	uint16_t address;
	uint8_t length;
//char fieldName[10]
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

static void* poolObject;
static unsigned long randomContext;

static rts_packet rtsPacketTemplate;
static cts_packet ctsPacketTemplate;
static acknowledge_packet ackPacketTemplate;

static block_handler dataHandler;

static void ConnectionEstablished();
static bool SendPacket(void* packet, uint8_t length);
static bool SendPacketWithData(void* packet, uint8_t packetLength, void* data, uint8_t dataLength);
static void FrameReceived(void* data, uint8_t length);
static uint8_t FindNext(uint8_t destination);
static void UpdateCCA();
static bool IsChannelClear();

void Network_Initialize()
{
	FIFO_Initialize(frameQueue, MESSAGE_QUEUE_SIZE);

	poolObject = EventDispatcher_RegisterPublisher(0);

	RadioDriver_Initialize(FrameReceived);

	srand(RadioDriver_GetRandomNumber());
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

void Network_SendData(uint8_t receiver, void* data, uint8_t length)
{
	data_packet p;
	p.link.source = assignedNodeId;
	p.link.type = TYPE_DATA;
	p.network.receiver = receiver;
	p.network.sender = assignedNodeId;
	p.length = length;

	SendPacketWithData(&p, sizeof(p), data, length);
}

void Network_SendSensorData(uint8_t receiver, uint8_t sensorId, void* data, uint8_t length)
{
	sensor_data_packet p;
	p.link.source = assignedNodeId;
	p.link.type = TYPE_DATA;
	p.network.receiver = receiver;
	p.network.sender = assignedNodeId;
	p.sensorId = sensorId;
	p.length = length;

	SendPacketWithData(&p, sizeof(p), data, length);
}

// Internal

enum
{
	STATE_UNSYNCHRONIZED,
	STATE_IDLE,
	STATE_EXPECTING_CTS,
	STATE_EXPECTING_ACK,
	STATE_EXPECTING_DATA,
	STATE_EXPECTING_PACKET
};
static volatile uint8_t state;
static uint8_t currentLink;
static uint8_t currentFrameLength;
static uint8_t currentFrame[NETWORK_MAXIMUM_FRAME_SIZE];

static void ConnectionEstablished()
{
	rtsPacketTemplate.link.source = assignedNodeId;
	rtsPacketTemplate.link.type = TYPE_RTS;
	ctsPacketTemplate.link.source = assignedNodeId;
	ctsPacketTemplate.link.type = TYPE_CTS;
	ackPacketTemplate.link.source = assignedNodeId;
	ackPacketTemplate.link.type = TYPE_ACK;
}

static bool SendPacket(void* frame, uint8_t length)
{
	if (FIFO_GetFreeSpace(frameQueue) >= (1 + length))
	{
		FIFO_WriteByte(frameQueue, length);
		FIFO_Write(frameQueue, frame, length);
		return true;
	}
	else
	{
		// error - buffer overflow
		return false;
	}
}

static bool SendPacketWithData(void* frame, uint8_t frameLength, void* data, uint8_t dataLength)
{
	if (FIFO_GetFreeSpace(frameQueue) >= (1 + frameLength + dataLength))
	{
		FIFO_WriteByte(frameQueue, frameLength + dataLength);
		FIFO_Write(frameQueue, frame, frameLength);
		FIFO_Write(frameQueue, data, dataLength);
		return true;
	}
	else
	{
		// error - buffer overflow
		return false;
	}
}

static void InitiateSynchronization()
{
	while (state != STATE_IDLE)
		;

	state = STATE_UNSYNCHRONIZED;
}

void Network_TimerEvent()
{
	if (state == STATE_UNSYNCHRONIZED)
	{
		return;
	}

	UpdateCCA();

	switch (state)
	{
		case STATE_EXPECTING_CTS:
			// RTS was never replied to. Inform router of bad node.
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

	if (currentFrameLength != 0 || FIFO_IsEmpty(frameQueue) == false) // frames to send
	{
		uint8_t slot = rand_r(&randomContext) & 0x3;
		for (uint8_t i = 0; i < slot; i++)
		{
			if (IsChannelClear() == false)
			{
				break;
			}
			_delay_us(RTS_DELAY_SLOT_DURATION);
		}

		if (IsChannelClear())
		{
			if (currentFrameLength == 0) // has the frame already been loaded? (by a previous transmission attempt)
			{
				currentFrameLength = FIFO_ReadByte(frameQueue);
				FIFO_Read(frameQueue, currentFrame, currentFrameLength);
			}

			// Should not be cached as the route may change
			link_network_header* lnh = (link_network_header*) currentFrame;
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
			RadioDriver_EnableReceiveMode();
		}
	}
	else // nothing to send => receive
	{
		for (uint8_t i = 0; i < 4; i++)
		{
			if (IsChannelClear() == false)
			{
				state = STATE_EXPECTING_PACKET;
				RadioDriver_EnableReceiveMode();
				break;
			}
			_delay_us(RTS_DELAY_SLOT_DURATION);
		}
	}


	// process low resolution, high precision timers
}

static void FrameReceived(void* data, uint8_t length)
{
	link_header* lh = data;
	link_network_header* lnh = data;

	if (lh->destination == assignedNodeId || lh->destination == BROADCAST_ID)
	{
		switch (lh->type)
		{
			// Non broadcast and non routable
			case TYPE_RTS: // RTS is never broadcasted
				{
					if (state == STATE_UNSYNCHRONIZED)
					{
						rts_packet* p = data;
						uint8_t slot = p->slot;
						// set timer
					}

					if (state == STATE_EXPECTING_PACKET || state == STATE_UNSYNCHRONIZED)
					{

						ctsPacketTemplate.link.destination = lh->source;
						RadioDriver_Send(&ctsPacketTemplate, sizeof(ctsPacketTemplate));

						currentLink = lh->source;
						state = STATE_EXPECTING_DATA;
					}
				}
				break;

			case TYPE_CTS: // CTS is never broadcasted
				{
					if (state == STATE_EXPECTING_CTS && currentLink == lh->source)
					{
						RadioDriver_Send(&currentFrame, currentFrameLength); // don't send the length indicator

						state = STATE_EXPECTING_ACK;
					}
					else
					{
						state = STATE_IDLE;
					}
				}
				break;

			case TYPE_ACK:
				{
					if (state == STATE_EXPECTING_ACK && currentLink == lh->source)
					{
						currentFrameLength = 0;

						state = STATE_IDLE;


						// signal packet sent event
					}
				}
				break;


				//			case TYPE_SENSOR_DATA:
				//				{
				//				}
				//				break;


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
						}
						else // forward data
						{
							lnh->link.source = assignedNodeId;
							SendPacket(data, length);
						}
					}

					state = STATE_IDLE;
				}
				break;

			case TYPE_SET:
				{
					//set_packet* p = (set_packet*) f->payload;
				}
				break;


				//			case TYPE_SET_COMPLETE:
				//				{
				//					set_complete_packet* p = (set_complete_packet*) f->payload;
				//				}
				//				break;

			case TYPE_GET:
				{
					//	get_packet* p = (get_packet*) f->payload;
				}
				break;


				//			case TYPE_GET_COMPLETE:
				//				{
				//					get_complete_packet* p = (get_complete_packet*) f->payload;
				//				}
				//				break;

			case TYPE_WRITE:
				{
					//	write_packet* p = (write_packet*) f->payload;
				}
				break;


				//			case TYPE_WRITE_COMPLETE:
				//				{
				//					write_complete_packet* p = (write_complete_packet*) f->payload;
				//				}
				//				break;

			case TYPE_READ:
				{
					//		read_packet* p = (read_packet*) f->payload;
				}
				break;


				//			case TYPE_WRITE_COMPLETE:
				//				{
				//					read_complete_packet* p = (read_complete_packet*) f->payload;
				//				}
				//				break;
		}
	}
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
static int8_t ccaThreshold = 0;

static void UpdateCCA()
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
