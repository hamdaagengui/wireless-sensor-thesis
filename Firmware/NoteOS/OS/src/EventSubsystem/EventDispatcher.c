/*
 * EventDispatcher.c
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#include "EventDispatcher.h"
#include "../Collections/Queue.h"
#include "../MemorySubsystem/MemoryManager.h"
#include "../Diagnostics/Diagnostics.h"

typedef struct
{
	sensor_id sensor;
	event_handler handler;
} subscriber;
static subscriber subscribers[EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS];
static uint8_t numberOfSubscribers;

enum
{
	TYPE_PUBLICATION,
	TYPE_NOTIFICATION,
	//	TYPE_COMPLETION,
	TYPE_PROCESSING
};

typedef struct
{
	uint8_t type;
	union
	{
		struct
		{
			sensor_id sensor;
			void* data;
		} publication;
		struct
		{
			notification_handler handler;
		} notification;
		//		struct
		//		{
		//			completion_handler handler;
		//			void* operation;
		//		} completion;
		struct
		{
			block_handler handler;
			void* data;
			uint8_t length;
		} processing;
	};
} event;

static uint8_t eventQueue[Queue_CalculateSize(sizeof(event), EVENTDISPATCHER_QUEUE_SIZE)];

void EventDispatcher_Initialize()
{
	Queue_Initialize(eventQueue, sizeof(event), EVENTDISPATCHER_QUEUE_SIZE);
}

void EventDispatcher_Dispatch()
{
	while (Queue_IsEmpty(eventQueue) == false)
	{
		event* e = Queue_Tail(eventQueue);

		switch (e->type)
		{
			case TYPE_PUBLICATION:
				for (uint8_t i = 0; i < numberOfSubscribers; i++)
				{
					if (subscribers[i].sensor == e->publication.sensor)
					{
						subscribers[i].handler(e->publication.sensor, e->publication.data);
					}
				}

				MemoryManager_Release(e->publication.data);

				Diagnostics_SendEvent(DIAGNOSTICS_PUBLISH_EXECUTED);
				break;

			case TYPE_NOTIFICATION:
				e->notification.handler();

				Diagnostics_SendEvent(DIAGNOSTICS_NOTIFY_EXECUTED);
				break;

			case TYPE_PROCESSING:
				e->processing.handler(e->processing.data, e->processing.length);

				MemoryManager_Release(e->processing.data);

				Diagnostics_SendEvent(DIAGNOSTICS_PROCESS_EXECUTED);
				break;
		}

		Queue_AdvanceTail(eventQueue);
	}
}

void EventDispatcher_Subscribe(sensor_id sensor, event_handler handler)
{
	if (numberOfSubscribers >= EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_TOO_MANY_EVENT_SUBSCRIBERS);
		return;
	}

	subscribers[numberOfSubscribers].sensor = sensor;
	subscribers[numberOfSubscribers].handler = handler;
	numberOfSubscribers++;
}

bool EventDispatcher_Publish(sensor_id sensor, void* data)
{
	if (data == NULL)
	{
		return false;
	}

	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_QUEUE_OVERFLOW);
		return false;
	}

	event* e = Queue_Head(eventQueue);

	e->type = TYPE_PUBLICATION;
	e->publication.sensor = sensor;
	e->publication.data = data;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_PUBLISH_QUEUED);

	return true;
}

bool EventDispatcher_Notify(notification_handler handler)
{
	if (handler == NULL)
	{
		return false;
	}

	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_QUEUE_OVERFLOW);
		return false;
	}

	event* e = Queue_Head(eventQueue);

	e->type = TYPE_NOTIFICATION;
	e->notification.handler = handler;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_NOTIFY_QUEUED);

	return true;
}

//bool EventDispatcher_Complete(completion_handler handler, void* operation)
//{
//	if (handler == NULL)
//	{
//		return false;
//	}
//
//	if (Queue_IsFull(eventQueue)) // queue full ?
//	{
//		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_QUEUE_OVERFLOW);
//		return false;
//	}
//
//	event* e = Queue_Head(eventQueue);
//
//	e->type = TYPE_COMPLETION;
//	e->completion.handler = handler;
//	e->completion.operation = operation;
//
//	Queue_AdvanceHead(eventQueue);
//
//	Diagnostics_SendEvent(DIAGNOSTICS_NOTIFY_QUEUED);
//
//	return true;
//}

bool EventDispatcher_Process(block_handler handler, void* data, uint8_t length)
{
	if (handler == NULL || data == NULL || length == 0)
	{
		return false;
	}

	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_QUEUE_OVERFLOW);
		return false;
	}

	event* e = Queue_Head(eventQueue);

	e->type = TYPE_PROCESSING;
	e->processing.handler = handler;
	e->processing.data = data;
	e->processing.length = length;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_PROCESS_QUEUED);

	return true;
}
