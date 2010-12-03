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
	event e;
	event_handler handler;
} subscriber;
static subscriber subscribers[EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS];
static uint8_t numberOfSubscribers;

enum
{
	TYPE_PUBLICATION,
	TYPE_COMPLETION,
	TYPE_PROCESSING
};

typedef struct
{
	uint8_t type;
	union
	{
		struct
		{
			event e;
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
} queued_event;

static uint8_t eventQueue[Queue_CalculateSize(sizeof(queued_event), EVENTDISPATCHER_QUEUE_SIZE)];

void EventDispatcher_Initialize()
{
	Queue_Initialize(eventQueue, sizeof(queued_event), EVENTDISPATCHER_QUEUE_SIZE);
}

void EventDispatcher_Dispatch()
{
	while (Queue_IsEmpty(eventQueue) == false)
	{
		queued_event* qe = Queue_Tail(eventQueue);

		switch (qe->type)
		{
			case TYPE_PUBLICATION:
				Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_PUBLISH_EXECUTED);

				for (uint8_t i = 0; i < numberOfSubscribers; i++)
				{
					if (subscribers[i].e == qe->publication.e)
					{
						subscribers[i].handler(qe->publication.e, qe->publication.data);
					}
				}

				if (qe->publication.data != NULL)
				{
					MemoryManager_Release(qe->publication.data);
				}
				break;

			case TYPE_COMPLETION:
				Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_COMPLETION_EXECUTED);

				qe->notification.handler();
				break;

			case TYPE_PROCESSING:
				Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_PROCESS_EXECUTED);

				qe->processing.handler(qe->processing.data, qe->processing.length);

				MemoryManager_Release(qe->processing.data);
				break;
		}

		Queue_AdvanceTail(eventQueue);
	}
}

void EventDispatcher_Subscribe(event e, event_handler handler)
{
	if (numberOfSubscribers >= EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS)
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_TOO_MANY_EVENT_SUBSCRIBERS);
		return;
	}

	subscribers[numberOfSubscribers].e = e;
	subscribers[numberOfSubscribers].handler = handler;
	numberOfSubscribers++;
}

bool EventDispatcher_Publish(event e, void* data)
{
	if (Queue_IsFull(eventQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_QUEUE_OVERFLOW);
		return false;
	}

	queued_event* qe = Queue_Head(eventQueue);

	qe->type = TYPE_PUBLICATION;
	qe->publication.e = e;
	qe->publication.data = data;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_PUBLISH_QUEUED);

	return true;
}

bool EventDispatcher_Complete(completion_handler handler)
{
	if (handler == NULL)
	{
		return false;
	}

	if (Queue_IsFull(eventQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_QUEUE_OVERFLOW);
		return false;
	}

	queued_event* qe = Queue_Head(eventQueue);

	qe->type = TYPE_COMPLETION;
	qe->notification.handler = handler;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_COMPLETION_QUEUED);

	return true;
}

bool EventDispatcher_Process(block_handler handler, void* data, uint8_t length)
{
	if (handler == NULL || data == NULL || length == 0)
	{
		return false;
	}

	if (Queue_IsFull(eventQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_QUEUE_OVERFLOW);
		return false;
	}

	queued_event* qe = Queue_Head(eventQueue);

	qe->type = TYPE_PROCESSING;
	qe->processing.handler = handler;
	qe->processing.data = data;
	qe->processing.length = length;

	Queue_AdvanceHead(eventQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_EVENT_DISPATCHER_PROCESS_QUEUED);

	return true;
}
