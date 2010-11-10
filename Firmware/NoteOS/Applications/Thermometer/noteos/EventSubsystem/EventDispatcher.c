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

// TODO EVENTDISPATCHER_HIGHEST_EVENT_ID should be replaced by EVENT_LAST_ID or something like that so the user shouldn't explicitly define it.
static event_handler subscribers[EVENTDISPATCHER_HIGHEST_EVENT_ID][EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS];

enum
{
	TYPE_NOTIFICATION,
	TYPE_PROCESSING,
	TYPE_PUBLICATION
};

typedef struct
{
	uint8_t type;
	union
	{
		struct
		{
			completion_handler handler;
		} notification;
		struct
		{
			block_handler handler;
			void* data;
			uint8_t length;
		} processing;
		struct
		{
			uint8_t event;
			void* data;
		} publication;
	};
} queue_element;

static uint8_t eventQueue[Queue_CalculateSize(sizeof(queue_element), EVENTDISPATCHER_QUEUE_SIZE)];

void EventDispatcher_Initialize()
{
	Queue_Initialize(eventQueue, sizeof(queue_element), EVENTDISPATCHER_QUEUE_SIZE);
}

void EventDispatcher_Dispatch()
{
	while (Queue_IsEmpty(eventQueue) == false)
	{
		queue_element* qe = Queue_Tail(eventQueue);

		switch (qe->type)
		{
			case TYPE_NOTIFICATION:
				qe->notification.handler();

				Diagnostics_SendEvent(DIAGNOSTICS_NOTIFY_EXECUTED);
				break;

			case TYPE_PROCESSING:
				qe->processing.handler(qe->processing.data, qe->processing.length);

				MemoryManager_Release(qe->processing.data);

				Diagnostics_SendEvent(DIAGNOSTICS_PROCESS_EXECUTED);
				break;

			case TYPE_PUBLICATION:
				for (uint8_t i = 0; i < EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS; i++)
				{
					if (subscribers[qe->publication.event][i] == NULL)
					{
						break; // no more subscribers
					}

					subscribers[qe->publication.event][i](qe->publication.data);
				}

				MemoryManager_Release(qe->publication.data);

				Diagnostics_SendEvent(DIAGNOSTICS_PUBLISH_EXECUTED);
				break;
		}

		Queue_AdvanceTail(eventQueue);
	}
}

void EventDispatcher_Subscribe(uint8_t event, event_handler handler)
{
	for (uint8_t i = 0; i < EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS; i++)
	{
		if (subscribers[event][i] == NULL)
		{
			subscribers[event][i] = handler;
			return;
		}
	}

	Diagnostics_SendEvent(DIAGNOSTICS_TOO_MANY_EVENT_SUBSCRIBERS);
}

void EventDispatcher_Notify(completion_handler handler)
{
	if (handler == NULL)
	{
		return;
	}

	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_QUEUE_OVERFLOW);
		return;
	}

	queue_element* qe = Queue_Head(eventQueue);

	qe->type = TYPE_NOTIFICATION;
	qe->notification.handler = handler;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_NOTIFY_QUEUED);
}

void EventDispatcher_Process(block_handler handler, void* data, uint8_t length)
{
	if (handler == NULL || data == NULL || length == 0)
	{
		return;
	}

	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_QUEUE_OVERFLOW);
		return;
	}

	queue_element* qe = Queue_Head(eventQueue);

	qe->type = TYPE_PROCESSING;
	qe->processing.handler = handler;
	qe->processing.data = data;
	qe->processing.length = length;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_PROCESS_QUEUED);
}

void EventDispatcher_Publish(uint8_t event, void* data)
{
	if (data == NULL)
	{
		return;
	}

	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_QUEUE_OVERFLOW);
		return;
	}

	queue_element* qe = Queue_Head(eventQueue);

	qe->type = TYPE_PUBLICATION;
	qe->publication.event = event;
	qe->publication.data = data;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_PUBLISH_QUEUED);
}
