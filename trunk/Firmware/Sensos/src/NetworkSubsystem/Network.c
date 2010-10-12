/*
 * Network.c
 *
 *  Created on: 10/09/2010
 *      Author: Coma
 */
//
#include <avr/eeprom.h>
#include <string.h>
#include "Network.h"
#include "../HardwareAbstractionLayer/RadioDriver.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../Collections/FIFO.h"
#include "../HardwareAbstractionLayer/NetworkTimer.h"
#include "../HardwareAbstractionLayer/NonVolatileStorage.h"
//
#define ROUTE_ENTRY_TIMEOUT																					100
#define MAX_FRAME_SIZE																							64
#define QUEUE_APPLICATION																						0
#define QUEUE_NETWORK																								1
#define QUEUE_SIZE																									50
#define NETWORK_FRAMES_PER_SECOND																		32
#define NETWORK_TICKS_PER_FRAME																			1024
#define NETWORK_TICKS_PER_SECOND																		(NETWORK_FRAMES_PER_SECOND * NETWORK_TICKS_PER_FRAME)
#define NETWORK_INITIAL_DELAY																				100
#define NETWORK_MASTER_NODE_TIME_SLOT_LENGTH												42
//
EEMEM uint32_t eeSerialNumber;

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
	uint8_t rssis[16];
	uint8_t sequenceNumber;
	uint8_t age;
} rssi;
static rssi rssis[16];

static uint8_t routeTable[16];
static uint16_t delayValues[16];

static uint8_t currentTimeSlot = 1;
static uint8_t activeTimeSlots = 1;
static uint8_t timeSlotLengths[16];
static uint16_t unallocatedFrameTime;

static uint8_t slotAllocationSequenceNumber;
#ifdef NETWORK_MASTER_NODE
static uint8_t slotAllocationSequenceNumbers[16];
#endif

// Network layer transmission queue
uint8_t messageQueue[FIFO_CalculateSize(MESSAGE_QUEUE_SIZE)];

// The raw network frame as sent to the radio driver
typedef struct
{
	uint8_t source :4; // source node of this frame
	uint8_t :4;
	uint8_t messages[];
} network_frame;

// Messages
enum
{
	MESSAGE_CONFIGURATION,
	MESSAGE_CONFIGURATION_ACKNOWLEDGE,
	MESSAGE_SLOT_ALLOCATIONS,
	MESSAGE_SLOT_ALLOCATIONS_ACKNOWLEDGE,

	MESSAGE_NEIGHBOR_REPORT,
	MESSAGE_NODE_STATE,

	MESSAGE_SENSOR_DATA,
	MESSAGE_SENSOR_DATA_ACKNOWLEDGE
};

typedef struct
{
	uint8_t msgId :4;
} base_message;

// Configuration messages
typedef struct
{
	uint8_t msgId :4;
	uint8_t slot :4;
	uint32_t serialNumber;
} configuration_message;

typedef struct
{
	uint8_t msgId :4;
	uint8_t :4;
} configuration_acknowledge_message;

typedef struct
{
	uint8_t msgId :4;
	uint8_t sequenceNumber :4;
	uint8_t slots[16];
} slot_allocations_message;

typedef struct
{
	uint8_t msgId :4;
	uint8_t sequenceNumber :4;
	uint8_t node :4;
	uint8_t hop :4;
} slot_allocations_acknowledge_message;

// Network messages
typedef struct
{
	uint8_t msgId :4;
	uint8_t sequenceNumber :4;
	uint8_t node :4;
	uint8_t :4;
	int8_t rssis[16];
} neighbor_report_message;

typedef struct
{
	uint8_t msgId :4;
	uint8_t sequenceNumber :4;
	uint8_t node :4;
	uint8_t :4;
	int8_t txLevel;
	uint8_t energyLevel;
	uint8_t queueLevel;
	uint8_t busyLevel;
} node_state_message;

// Sensor messages
typedef struct
{
	uint8_t msgId :4;
	uint8_t sequenceNumber :4;
	uint8_t hop :4;
	uint8_t length :4;
	uint8_t sensor;
	uint8_t data[];
} sensor_data_message;

typedef struct
{
	uint8_t msgId :4;
	uint8_t sequenceNumber :4;
	uint8_t hop :4;
	uint8_t destination :4;
} sensor_data_acknowledge_message;

static uint8_t assignedSlot;

enum
{
	NETWORK_STATE_UNINITIALIZED,
	NETWORK_STATE_INACTIVE,
	NETWORK_STATE_ACTIVE,
	NETWORK_STATE_SYNCHRONIZING
};
static uint8_t networkState;

//static uint8_t queues[2][QUEUE_SIZE];

static void* poolObject;

void Network_SynchronizeTimer();
static void FrameReceived(uint8_t* data, uint8_t length);
static uint8_t GetNextNodeInRouteToNodeZero();
static uint8_t GetNextNodeInRouteToNode(uint8_t destination);
static void TimerTick();
static bool SendMessage(void* msg, uint8_t length);
static bool SendMessageWithData(void* msg, uint8_t length, void* data, uint8_t dataLength);

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

	timeSlotLengths[0] = NETWORK_MASTER_NODE_TIME_SLOT_LENGTH;
	unallocatedFrameTime = NETWORK_TICKS_PER_FRAME - NETWORK_MASTER_NODE_TIME_SLOT_LENGTH;

	NetworkTimer_Initialize(TimerTick);


#ifdef NETWORK_MASTER_NODE
	NetworkTimer_SetTimerPeriod(NETWORK_INITIAL_DELAY);
#else
	Network_SynchronizeTimer();
#endif
}

#ifndef NETWORK_MASTER_NODE
enum
{
	TIMER_STATE_UNSYNCHRONIZED,
	TIMER_STATE_SYNCHRONIZING,
	TIMER_STATE_SYNCHRONIZED,
};
static uint8_t timerState;

static void RxStartReceived()
{
	NetworkTimer_SetTimerValue(1);
	NetworkTimer_SetTimerPeriod(255);
}

void Network_SynchronizeTimer()
{
	timerState = TIMER_STATE_SYNCHRONIZING;

	RadioDriver_SetRxStartHandler(RxStartReceived);
}
#endif

#ifndef NETWORK_MASTER_NODE
void Network_SendSensorData(uint8_t sensorId, void* data, uint8_t length)
{
	// add to transport layer queue
}
#endif

static void DoSend()
{
	uint8_t frame[MAX_FRAME_SIZE];

	network_frame* nf = (network_frame*) frame;
	nf->source = assignedSlot;

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

static void TimerTick()
{
#ifndef NETWORK_MASTER_NODE
	if (timerState != TIMER_STATE_SYNCHRONIZED)
	{
		return;
	}
#endif

	if (++currentTimeSlot > activeTimeSlots)
	{
		currentTimeSlot = 0;
	}

	if (currentTimeSlot == assignedSlot)
	{
		DoSend();
	}
	else if (currentTimeSlot < activeTimeSlots)
	{
		// if node is visible from this node (rssi != 0) enable rx in case node use us for route
		//   disable rx if rx start was not received within timeout limit
	}

	if (currentTimeSlot == activeTimeSlots)
	{
		NetworkTimer_SetTimerPeriod(unallocatedFrameTime);
	}
	else
	{
		NetworkTimer_SetTimerPeriod(timeSlotLengths[currentTimeSlot]);
	}
}

static void FrameReceived(uint8_t* data, uint8_t length)
{
	uint8_t source = ((network_frame*) data)->source;


#ifndef NETWORK_MASTER_NODE
	if (timerState == TIMER_STATE_SYNCHRONIZING)
	{
		currentTimeSlot = source;
		NetworkTimer_SetTimerPeriod(timeSlotLengths[source]);
		timerState = TIMER_STATE_SYNCHRONIZED;
		RadioDriver_SetRxStartHandler(NULL);
	}
#endif

	for (uint8_t i = sizeof(network_frame); i < length;) // process all messages in the frame
	{
		void* currentMsg = &data[i];
		base_message* baseMsg = currentMsg;

		switch (baseMsg->msgId)
		{
			// Configuration

			case MESSAGE_CONFIGURATION:
				{
#ifndef NETWORK_MASTER_NODE
					configuration_message* m = currentMsg;

					uint32_t sn;
					NonVolatileStorage_Read(&eeSerialNumber, &sn, sizeof(sn));

					if (m->serialNumber == sn) // for this node?

					{
						assignedSlot = m->slot;

						networkState = NETWORK_STATE_INACTIVE;

						configuration_acknowledge_message ack; // send acknowledge
						ack.msgId = MESSAGE_CONFIGURATION_ACKNOWLEDGE;
						SendMessage(&ack, sizeof(configuration_acknowledge_message));
					}
#endif

					i += sizeof(configuration_message);
				}
				break;

			case MESSAGE_CONFIGURATION_ACKNOWLEDGE:
				{
#ifdef NETWORK_MASTER_NODE
					// config was acknowledged by <source>
					EventDispatcher_Publish(EVENT_NODE_CONNECTED, NULL);
#endif

					i += sizeof(configuration_acknowledge_message);
				}
				break;

			case MESSAGE_SLOT_ALLOCATIONS:
				{
#ifndef NETWORK_MASTER_NODE
					slot_allocations_message* m = currentMsg;

					if (m->sequenceNumber != slotAllocationSequenceNumber)
					{
						slotAllocationSequenceNumber = m->sequenceNumber;

						SendMessage(m, sizeof(slot_allocations_message)); // forward message to all

						uint16_t delay = 0;
						delayValues[assignedSlot] = 0;
						delayValues[assignedSlot - 1] = 0;
						for (int8_t i = (assignedSlot - 2); i >= 0; i--)
						{
							delay += m->slots[i + 1];
							delayValues[i] = delay;
						}

						slot_allocations_acknowledge_message ack; // send acknowledge
						ack.msgId = MESSAGE_SLOT_ALLOCATIONS_ACKNOWLEDGE;
						ack.sequenceNumber = slotAllocationSequenceNumber;
						ack.node = assignedSlot;
						ack.hop = GetNextNodeInRouteToNodeZero();
						SendMessage(&ack, sizeof(slot_allocations_acknowledge_message));
					}
#endif

					i += sizeof(slot_allocations_message);
				}
				break;

			case MESSAGE_SLOT_ALLOCATIONS_ACKNOWLEDGE:
				{
					slot_allocations_acknowledge_message* m = currentMsg;


#ifdef NETWORK_MASTER_NODE
					// note that node <m->node> has received the updated slot allocations
					if (m->sequenceNumber == slotAllocationSequenceNumber)
					{
						slotAllocationSequenceNumbers[m->node] = slotAllocationSequenceNumber;
						// check if all have sent ack now
						//   stop sending out slot allocation updates
					}
#else
					if (m->hop == assignedSlot) // hopping on me?

					{
						m->hop = GetNextNodeInRouteToNodeZero(); // forward message
						SendMessage(m, sizeof(slot_allocations_acknowledge_message));
					}
#endif

					i += sizeof(slot_allocations_acknowledge_message);
				}
				break;


				// Routing

			case MESSAGE_NEIGHBOR_REPORT:
				{
					neighbor_report_message* m = currentMsg;
					rssi* r = &rssis[m->node];

					if (m->sequenceNumber != r->sequenceNumber) // skip if message is obsolete
					{
						r->sequenceNumber = m->sequenceNumber;

						SendMessage(m, sizeof(neighbor_report_message)); // forward message to all

						for (uint8_t i = 0; i < 16; i++)
						{
							r->rssis[i] = m->rssis[i];
						}

						r->age = 0;
					}

					i += sizeof(neighbor_report_message);
				}
				break;

			case MESSAGE_NODE_STATE:
				{
					node_state_message* m = currentMsg;
					node_state* ns = &nodeStates[m->node];

					if (m->sequenceNumber != ns->sequenceNumber) // skip if message is obsolete
					{
						ns->sequenceNumber = m->sequenceNumber;

						SendMessage(m, sizeof(node_state_message)); // forward message to all

						ns->txLevel = m->txLevel;
						ns->energyLevel = m->energyLevel;
						ns->queueLevel = m->queueLevel;
						ns->busyLevel = m->busyLevel;

						ns->age = 0;
					}

					i += sizeof(node_state_message);
				}
				break;


				// Transport

			case MESSAGE_SENSOR_DATA:
				{
					sensor_data_message* m = currentMsg;


#ifdef NETWORK_MASTER_NODE
					if ((sizeof(sensor_event_report) + m->length) > EVENTDISPATCHER_REPORT_DATA_SIZE)
					{
						// error
					}
					else
					{
						sensor_event_report* r = poolObject;
						r->sensorId = m->sensor;
						r->length = m->length;
						memcpy(r->data, m->data, m->length);
						poolObject = EventDispatcher_Publish(EVENT_SENSOR_DATA_RECEIVED, poolObject);
					}
#else
					if (m->hop == assignedSlot) // hopping on me?

					{
						m->hop = GetNextNodeInRouteToNodeZero(); // forward message
						SendMessage(m, sizeof(sensor_data_message));
					}
#endif

					i += sizeof(sensor_data_message) + m->length;
				}
				break;

			case MESSAGE_SENSOR_DATA_ACKNOWLEDGE:
				{
#ifndef NETWORK_MASTER_NODE
					sensor_data_acknowledge_message* m = currentMsg;

					if (m->hop == assignedSlot) // hopping on me?

					{
						m->hop = GetNextNodeInRouteToNodeZero(); // forward message
						SendMessage(m, sizeof(sensor_data_acknowledge_message));
					}
#endif

					i += sizeof(sensor_data_acknowledge_message);
				}
				break;

			default: // error -> abort
				i = length;
				break;
		}
	}
}

static void UpdateRouteTable()
{

}

static uint8_t GetNextNodeInRouteToNodeZero()
{
	return 0;
}

static uint8_t GetNextNodeInRouteToNode(uint8_t destination)
{
	return 0;
}

static bool SendMessage(void* msg, uint8_t length)
{
	if (FIFO_GetFreeSpace(messageQueue) >= (1 + length))
	{
		FIFO_WriteByte(messageQueue, length);
		FIFO_Write(messageQueue, msg, length);
		return true;
	}
	else
	{
		// error - buffer overflow
		return false;
	}
}

static bool SendMessageWithData(void* msg, uint8_t length, void* data, uint8_t dataLength)
{
	if (FIFO_GetFreeSpace(messageQueue) >= (1 + length + dataLength))
	{
		FIFO_WriteByte(messageQueue, length);
		FIFO_Write(messageQueue, msg, length);
		FIFO_Write(messageQueue, data, dataLength);
		return true;
	}
	else
	{
		// error - buffer overflow
		return false;
	}
}
