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
	TYPE_PUBLICATION,
	TYPE_NOTIFICATION,
	TYPE_PROCESSING
};

typedef struct
{
	uint8_t type;
	union
	{
		struct
		{
			uint8_t event;
			void* data;
		} publication;
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
				for (uint8_t i = 0; i < EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS; i++)
				{
					if (subscribers[e->publication.event][i] == NULL)
					{
						break; // no more subscribers
					}

					subscribers[e->publication.event][i](e->publication.event, e->publication.data);
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

void EventDispatcher_Subscribe(uint8_t eventId, event_handler handler)
{
	for (uint8_t i = 0; i < EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS; i++)
	{
		if (subscribers[eventId][i] == NULL)
		{
			subscribers[eventId][i] = handler;
			return;
		}
	}

	Diagnostics_SendEvent(DIAGNOSTICS_TOO_MANY_EVENT_SUBSCRIBERS);
}

bool EventDispatcher_Publish(uint8_t eventId, void* data)
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
	e->publication.event = eventId;
	e->publication.data = data;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_PUBLISH_QUEUED);

	return true;
}

bool EventDispatcher_Notify(completion_handler handler)
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
