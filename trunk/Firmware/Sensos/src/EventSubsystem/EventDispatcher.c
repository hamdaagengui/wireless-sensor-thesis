/*
 * EventDispatcher.c
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#include "EventDispatcher.h"
#include "../Collections/Pool.h"
#include "../Collections/Queue.h"

// TODO EVENTDISPATCHER_HIGHEST_EVENT_ID should be replaced by EVENT_LAST_ID or something like that so the user shouldn't explicitly define it.
static event_handler subscribers[EVENTDISPATCHER_HIGHEST_EVENT_ID][EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS];

typedef struct
{
	bool isNotification;
	union
	{
		struct
		{
			completion_handler handler;
		} notification;
		struct
		{
			uint8_t event;
			void* data;
		} publication;
	};
} queue_element;

static uint8_t eventQueue[Queue_CalculateSize(sizeof(queue_element), EVENTDISPATCHER_QUEUE_SIZE)];
static uint8_t bufferPool[Pool_CalculateSize(EVENTDISPATCHER_REPORT_DATA_SIZE, EVENTDISPATCHER_REPORT_DATA_POOL_SIZE)];

void EventDispatcher_Initialize()
{
	Queue_Initialize(eventQueue, sizeof(queue_element), EVENTDISPATCHER_QUEUE_SIZE);
	Pool_Initialize(bufferPool, EVENTDISPATCHER_REPORT_DATA_SIZE, EVENTDISPATCHER_REPORT_DATA_POOL_SIZE);
}

void EventDispatcher_Start()
{
}

void EventDispatcher_Dispatch()
{
	while (Queue_IsEmpty(eventQueue) == false)
	{
		queue_element* qe = Queue_Tail(eventQueue);

		if (qe->isNotification)
		{
			qe->notification.handler();
		}
		else
		{
			for (uint8_t i = 0; i < EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS; i++)
			{
				if (subscribers[qe->publication.event][i] != NULL)
				{
					subscribers[qe->publication.event][i](qe->publication.data);
				}
				else // no more subscribers
				{
					break;
				}
			}

			if (qe->publication.data != NULL)
			{
				Pool_ReleaseBlock(bufferPool, qe->publication.data);
			}
		}

		Queue_AdvanceTail(eventQueue);
	}
}

void EventDispatcher_RegisterSubscriber(uint8_t event, event_handler handler)
{
	for (uint8_t i = 0; i < EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS; i++)
	{
		if (subscribers[event][i] == NULL)
		{
			subscribers[event][i] = handler;
			return;
		}
	}


	// full => error
}

void* EventDispatcher_RegisterPublisher(uint8_t id)
{
	// return buffer => error if no buffers are available
	return Pool_AllocateBlock(bufferPool);
}

void* EventDispatcher_Publish(uint8_t event, void* data)
{
	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		// error
		return data; // just return the same buffer since it will not be passed on to event handlers
	}

#ifdef DEBUG
	if (event >= EVENTDISPATCHER_HIGHEST_EVENT_ID)
	{

	}
#endif

	queue_element* qe = Queue_Head(eventQueue);

	qe->isNotification = false;
	qe->publication.event = event;
	qe->publication.data = data;

	Queue_AdvanceHead(eventQueue);

	if (data != NULL)
	{
		// return new buffer => error if no buffers are available
		return Pool_AllocateBlock(bufferPool);
	}
	else
	{
		return NULL;
	}
}

void EventDispatcher_Notify(completion_handler handler)
{
	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		// error
		return;
	}

	queue_element* qe = Queue_Head(eventQueue);

	qe->isNotification = true;
	qe->notification.handler = handler;

	Queue_AdvanceHead(eventQueue);
}
