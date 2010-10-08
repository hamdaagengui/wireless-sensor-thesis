/*
 * Network.c
 *
 *  Created on: 10/09/2010
 *      Author: Coma
 */
#include "Network.h"
#include "../HardwareAbstractionLayer/RadioDriver.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../Collections/FIFO.h"
#include <avr/eeprom.h>
#include <string.h>

#define ROUTE_ENTRY_TIMEOUT					100
#define MAX_FRAME_SIZE							64

#define QUEUE_APPLICATION						0
#define QUEUE_NETWORK								1
#define QUEUE_SIZE									50

//EEMEM const uint8_t serialNumber[16];

void GetSerialNumber(uint8_t buffer[])
{
	buffer[0] = 123;

	for (uint8_t i = 1; i < 16; i++)
	{
		buffer[i] = 0;
	}
}

static bool isMasterNode = false;

// Device statistics
typedef struct
{
	uint8_t txLevel;
	uint8_t energyLevel;
	uint8_t queueLevel;
	uint8_t busyLevel;
	uint8_t sequenceNumber;
	uint8_t age;
} node_state;
static node_state nodeStates[16];

// Routing
typedef struct
{
	uint8_t rssi[16];
	uint8_t sequenceNumber;
	uint8_t age;
} rssi;
static rssi rssis[16];

static uint8_t routeTable[16];
static uint8_t nodesConnected = 0;
static uint8_t currentSlotAllocationMessageSequenceNumber = 0;
static uint16_t delayValues[16];

// Network layer transmission queue
uint8_t messageQueue[FIFO_CalculateSize(MESSAGE_QUEUE_SIZE)];

// The raw network frame as sent over the air
typedef struct
{
	uint8_t source :4; // source node of this frame
	uint8_t nodeCount :4; // number of sensor nodes that this device thinks exist in the network
	uint8_t messages[];
} network_frame;

// Messages
enum
{
	MESSAGE_NULL,
	MESSAGE_CONFIGURATION,
	MESSAGE_CONFIGURATION_ACKNOWLEDGE,
	MESSAGE_SLOT_ALLOCATIONS,
	MESSAGE_SENSOR_DATA,
	MESSAGE_SENSOR_DATA_ACKNOWLEDGE,
	MESSAGE_NEIGHBOR_REPORT,
	MESSAGE_NODE_STATE
};

typedef struct
{
	uint8_t id :4;
} base_message;

// Configuration messages
typedef struct
{
	uint8_t id :4;
	uint8_t slot :4;
	uint8_t serialNumber[16];
	uint8_t sequenceNumber :4;
	uint8_t :4;
	uint8_t slots[16];
} configuration_message;

typedef struct
{
	uint8_t id :4;
	uint8_t :4;
} configurationAcknowledge_message;

typedef struct
{
	uint8_t id :4;
	uint8_t sequenceNumber :4;
	uint8_t slots[16];
} slotAllocations_message;

typedef struct
{
	uint8_t id :4;
	uint8_t sequenceNumber :4;
	uint8_t slot :4;
	uint8_t :4;
} slotAllocationsAcknowledge_message;

// Sensor messages
typedef struct
{
	uint8_t id :4;
	uint8_t nextNode :4;
	uint8_t sequenceNumber :4;
	uint8_t length :4;
	uint8_t sensor;
	uint8_t data[];
} sensorData_message;

typedef struct
{
	uint8_t id :4;
	uint8_t nextNode :4;
	uint8_t destination :4;
	uint8_t sequenceNumber :4;
} sensorDataAcknowledge_message;

// Network messages
typedef struct
{
	uint8_t id :4;
	uint8_t node :4;
	uint8_t sequenceNumber :4;
	uint8_t :4;
	uint8_t rssi0 :5;
	uint8_t rssi1 :5;
	uint8_t rssi2 :5;
	uint8_t rssi3 :5;
	uint8_t rssi4 :5;
	uint8_t rssi5 :5;
	uint8_t rssi6 :5;
	uint8_t rssi7 :5;
	uint8_t rssi8 :5;
	uint8_t rssi9 :5;
	uint8_t rssi10 :5;
	uint8_t rssi11 :5;
	uint8_t rssi12 :5;
	uint8_t rssi13 :5;
	uint8_t rssi14 :5;
	uint8_t rssi15 :5;
} neighborReport_message;

typedef struct
{
	uint8_t id :4;
	uint8_t node :4;
	uint8_t sequenceNumber :4;
	uint8_t :4;
	uint8_t txLevel;
	uint8_t energyLevel;
	uint8_t queueLevel;
	uint8_t busyLevel;
} nodeState_message;

static uint8_t assignedSlot;

enum
{
	STATE_UNINITIALIZED,
	STATE_INACTIVE,
	STATE_ACTIVE,
	STATE_SYNCHRONIZING
};
static uint8_t state = STATE_UNINITIALIZED;

//static uint8_t queues[2][QUEUE_SIZE];

static void* poolObject;

static void FrameReceived(uint8_t* data, uint8_t length);
static uint8_t GetNextNodeTowardsZero();

//typedef void (*routeCalculatorProtoype)();
//typedef uint8_t (*routeFindingProtoype)();
//
//#define ROUTING_STRATEGY_CALCULATE_AT_UPDATE			0
//#define ROUTING_STRATEGY_CALCULATE_AT_REQUEST			1
//
//static uint8_t currentRoutingStrategy = ROUTING_STRATEGY_CALCULATE_AT_UPDATE;
//static routeCalculatorProtoype routeCalculators[2] = { };
//static routeFindingProtoype routeFinders[2] = { };

void Network_Initialize()
{
	FIFO_Initialize(messageQueue, MESSAGE_QUEUE_SIZE);

	poolObject = EventDispatcher_RegisterPublisher(0);

	RadioDriver_Initialize(FrameReceived);
}

void Network_SendSensorData(uint8_t sensorId, void* data, uint8_t length)
{
	// add to transport layer queue
}

static void DoSend()
{
	uint8_t frame[MAX_FRAME_SIZE];

	network_frame* nf = (network_frame*) frame;
	nf->source = assignedSlot;
	nf->nodeCount = nodesConnected;

	uint8_t length = sizeof(network_frame);

	while (FIFO_IsEmpty(messageQueue) == false)
	{
		uint8_t l = FIFO_PeekFirst(messageQueue);
		if ((length + l) <= MAX_FRAME_SIZE)
		{
			FIFO_Read(messageQueue, &frame[length], l);
			length += l;
		}
		else
		{
			break;
		}
	}

	if (length > sizeof(network_frame)) // more than just the header?
	{
		RadioDriver_Send(frame, length);
	}
}

static void FrameReceived(uint8_t* data, uint8_t length)
{
	uint8_t source = ((network_frame*) data)->source;
	uint8_t nodeCount = ((network_frame*) data)->nodeCount;

	if (nodeCount == 0)
	{
		void* currentMsg = &data[sizeof(network_frame)];
		base_message* baseMsg = currentMsg;

		if (baseMsg->id == MESSAGE_CONFIGURATION)
		{
			configuration_message* m = currentMsg;

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

			if (match) // for this node?
			{
				assignedSlot = m->slot;

				uint16_t delay = 0;
				delayValues[assignedSlot] = 0;
				delayValues[assignedSlot - 1] = 0;
				for (int8_t i = (assignedSlot - 2); i >= 0; i--)
				{
					delay += m->slots[i + 1];
					delayValues[i] = delay;
				}
				currentSlotAllocationMessageSequenceNumber = m->sequenceNumber;

				state = STATE_INACTIVE;

				configurationAcknowledge_message ack;
				ack.id = MESSAGE_CONFIGURATION_ACKNOWLEDGE;
				//ack.slot = assignedSlot;
				FIFO_WriteByte(messageQueue, sizeof(configurationAcknowledge_message));
				FIFO_Write(messageQueue, &ack, sizeof(configurationAcknowledge_message));
			}
		}
	}
	else
	{
		nodesConnected = nodeCount;

		if (state == STATE_SYNCHRONIZING) // waiting for sync to send queued messages?
		{
			if (source < assignedSlot) // a node before us?
			{
				if (source == (assignedSlot - 1)) // our turn to send?
				{
					DoSend();
				}
				else // no => wait before sending
				{
					uint16_t timeToWait = delayValues[source];
					// set timer
				}

				state = STATE_ACTIVE;
			}
			else // a node after us
			{

			}
		}

		for (uint8_t i = sizeof(network_frame); i < length;) // process all messages in the frame
		{
			void* currentMsg = &data[i];
			base_message* baseMsg = currentMsg;

			switch (baseMsg->id)
			{
				// Configuration

				case MESSAGE_CONFIGURATION:
					{
						configuration_message* m = currentMsg;

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

						if (match) // for this node?
						{
							assignedSlot = m->slot;

							uint16_t delay = 0;
							delayValues[assignedSlot] = 0;
							delayValues[assignedSlot - 1] = 0;
							for (int8_t i = (assignedSlot - 2); i >= 0; i--)
							{
								delay += m->slots[i + 1];
								delayValues[i] = delay;
							}
							currentSlotAllocationMessageSequenceNumber = m->sequenceNumber;

							state = STATE_INACTIVE;

							configurationAcknowledge_message ack;
							ack.id = MESSAGE_CONFIGURATION_ACKNOWLEDGE;
							//ack.slot = assignedSlot;
							FIFO_WriteByte(messageQueue, sizeof(configurationAcknowledge_message));
							FIFO_Write(messageQueue, &ack, sizeof(configurationAcknowledge_message));
						}

						i += sizeof(configuration_message);
					}
					break;

				case MESSAGE_CONFIGURATION_ACKNOWLEDGE:
					{
						if (isMasterNode)
						{
							// unused for now but should be implemented
						}

						i += sizeof(configurationAcknowledge_message);
					}
					break;

				case MESSAGE_SLOT_ALLOCATIONS:
					{
						slotAllocations_message* m = currentMsg;

						if (m->sequenceNumber > currentSlotAllocationMessageSequenceNumber) // skip if message is obsolete
						{
							uint16_t delay = 0;
							delayValues[assignedSlot] = 0;
							delayValues[assignedSlot - 1] = 0;
							for (int8_t i = (assignedSlot - 2); i >= 0; i--)
							{
								delay += m->slots[i + 1];
								delayValues[i] = delay;
							}
							currentSlotAllocationMessageSequenceNumber = m->sequenceNumber;

							FIFO_WriteByte(messageQueue, sizeof(slotAllocations_message));
							FIFO_Write(messageQueue, m, sizeof(slotAllocations_message));
						}

						i += sizeof(slotAllocations_message);
					}
					break;


					// Transport

				case MESSAGE_SENSOR_DATA:
					{
						sensorData_message* m = currentMsg;

						if (isMasterNode)
						{
							((uint8_t*) poolObject)[0] = m->length;
							((uint8_t*) poolObject)[1] = m->sensor;
							memcpy(poolObject + 2, m->data, m->length);
							poolObject = EventDispatcher_Publish(EVENT_SENSOR_DATA, poolObject);
						}
						else
						{
							if (m->nextNode == assignedSlot) // this node was selected as a hop
							{
								// forward towards node 0
								m->nextNode = GetNextNodeTowardsZero();
								// add to application layer queue
							}
						}

						i += sizeof(sensorData_message) + m->length;
					}
					break;

				case MESSAGE_SENSOR_DATA_ACKNOWLEDGE:
					{
						sensorDataAcknowledge_message* m = currentMsg;

						if (m->destination == assignedSlot)
						{

						}
						else if (m->nextNode == assignedSlot)
						{

						}

						i += sizeof(sensorDataAcknowledge_message);
					}
					break;


					// Routing

				case MESSAGE_NEIGHBOR_REPORT:
					{
						neighborReport_message* m = currentMsg;

						rssi* r = &rssis[m->node];

						if (m->sequenceNumber > r->sequenceNumber) // skip if message is obsolete
						{
							r->rssi[0] = m->rssi0;
							r->rssi[1] = m->rssi1;
							r->rssi[2] = m->rssi2;
							r->rssi[3] = m->rssi3;
							r->rssi[4] = m->rssi4;
							r->rssi[5] = m->rssi5;
							r->rssi[6] = m->rssi6;
							r->rssi[7] = m->rssi7;
							r->rssi[8] = m->rssi8;
							r->rssi[9] = m->rssi9;
							r->rssi[10] = m->rssi10;
							r->rssi[11] = m->rssi11;
							r->rssi[12] = m->rssi12;
							r->rssi[13] = m->rssi13;
							r->rssi[14] = m->rssi14;
							r->rssi[15] = m->rssi15;

							r->sequenceNumber = m->sequenceNumber;

							r->age = 0;

							FIFO_WriteByte(messageQueue, sizeof(neighborReport_message));
							FIFO_Write(messageQueue, m, sizeof(neighborReport_message));
						}

						i += sizeof(neighborReport_message);
					}
					break;

				case MESSAGE_NODE_STATE:
					{
						nodeState_message* m = currentMsg;

						node_state* ns = &nodeStates[m->node];

						if (m->sequenceNumber > ns->sequenceNumber) // skip if message is obsolete
						{
							ns->txLevel = m->txLevel;
							ns->energyLevel = m->energyLevel;
							ns->queueLevel = m->queueLevel;
							ns->busyLevel = m->busyLevel;

							ns->sequenceNumber = m->sequenceNumber;

							ns->age = 0;

							FIFO_WriteByte(messageQueue, sizeof(nodeState_message));
							FIFO_Write(messageQueue, m, sizeof(nodeState_message));
						}

						i += sizeof(nodeState_message);
					}
					break;

				default: // error -> abort
					i = length;
					break;
			}
		}
	}
}

static void UpdateRouteTable()
{

}

static uint8_t GetNextNodeTowardsZero()
{
	return 0;
}
