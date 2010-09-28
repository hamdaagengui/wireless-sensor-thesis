/*
 * Network.c
 *
 *  Created on: 10/09/2010
 *      Author: Coma
 */
#include "Network.h"
#include "HardwareAbstractionLayer/RadioDriver.h"
#include <avr/eeprom.h>

#define ROUTE_ENTRY_TIMEOUT      100
#define MAX_FRAME_SIZE

#define QUEUE_LP						0
#define QUEUE_HP						1

//EEMEM const uint8_t serialNumber[16];

void GetSerialNumber(uint8_t buffer[])
{
	buffer[0] = 123;

	for (uint8_t i = 1; i < 16; i++)
	{
		buffer[i] = 0;
	}
}

// Device statistics
//typedef struct
//{
//	uint8_t txPower;
//	uint8_t rssi;
//	uint8_t energyLevel;
//	uint8_t queueLevel;
//	uint8_t busyLevel;
//	uint8_t cost;
//	uint8_t age;
//} nodeInfo;
//
//nodeInfo nodes[16];

// Routing
typedef struct
{
	uint8_t distance;
	uint8_t age;
} distanceEntry;
static distanceEntry distances[16];

// Frames
typedef struct
{
	uint8_t destination :4;
	uint8_t source :4;
	uint8_t messages[];
} networkFrame;

typedef struct
{
	uint8_t length;
	networkFrame payload;
} bufferFrame;

// Messages
enum
{
	MESSAGE_CONFIGURATION,
	MESSAGE_DISTANCE,
	MESSAGE_SENSOR_DATA,
	MESSAGE_ACKNOWLEDGE
};

typedef struct
{
	uint8_t messageId :3;
	uint8_t :5;
} baseMessage;

typedef struct
{
	uint8_t messageId :3;
	uint8_t :1;
	uint8_t assignedId :4;
	uint8_t serialNumber[16];
} configurationMessage;

typedef struct
{
	uint8_t messageId :3;
	uint8_t :1;
	uint8_t distance :4;
} distanceMessage;

typedef struct
{
	uint8_t messageId :3;
	uint8_t sensorId :5;
	uint8_t length;
	uint8_t data[];
} sensorDataMessage;

typedef struct
{
	uint8_t messageId :3;
	uint8_t acknowledge :1;
	uint8_t :4;
} acknowledgeMessage;

static uint8_t assignedId;

enum
{
	STATE_UNCONNECTED,
	STATE_SYNCHRONIZING,
	STATE_CONNECTED
};
static uint8_t state = STATE_UNCONNECTED;

static uint8_t queues[2][QUEUE_SIZE];

static void FrameReceived(uint8_t* data, uint8_t length);

void Network_Initialize()
{
	RadioDriver_Initialize(FrameReceived);
}

void Network_Send(uint8_t channel, void* data, uint8_t length)
{

}

static void DoSend()
{

}

static void FrameReceived(uint8_t* data, uint8_t length)
{
	networkFrame* frame = (networkFrame*) data;
	uint8_t source = frame->source;
	uint8_t destination = frame->destination;

	for (uint8_t i = offsetof(networkFrame, messages); i < length;) // process all messages in the frame
	{
		void* currentMsg = &data[i];
		baseMessage* baseMsg = currentMsg;

		switch (baseMsg->messageId)
		{
			case MESSAGE_CONFIGURATION:
				{
					configurationMessage* m = currentMsg;

					uint8_t sn[16];
					GetSerialNumber(sn);

					bool match = true;
					for (uint8_t i = 0; i < 16; i++)
					{
						//if (m->serialNumber[i] != eeprom_read_byte((void*) i))
						if (m->serialNumber[i] != sn[i])
						{
							match = false;
							break;
						}
					}

					if (match)
					{
						assignedId = m->assignedId;
						state = STATE_CONNECTED;
						// send back ack?
					}

					i += sizeof(configurationMessage);
				}
				break;

			case MESSAGE_DISTANCE:
				{
					distanceMessage* m = currentMsg;

					distanceEntry* d = &distances[source];
					d->distance = m->distance;
					d->age = 0;

					i += sizeof(distanceMessage);
				}
				break;

			case MESSAGE_SENSOR_DATA:
				{
					sensorDataMessage* m = currentMsg;

					i += sizeof(sensorDataMessage);
				}
				break;

			case MESSAGE_ACKNOWLEDGE:
				{
					sensorDataMessage* m = currentMsg;

					i += sizeof(sensorDataMessage);
				}
				break;

			default: // error -> abort
				i = length;
				break;
		}
	}
}
