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
static eventHandler subscribers[EVENTDISPATCHER_HIGHEST_EVENT_ID][EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS];

typedef struct
{
	bool isNotification;
	union
	{
		struct
		{
			completionHandler handler;
		} notification;
		struct
		{
			uint8_t event;
			void* data;
		} publication;
	};
} queueElement;

static uint8_t eventQueue[Queue_CalculateSize(sizeof(queueElement), EVENTDISPATCHER_QUEUE_SIZE)];
static uint8_t bufferPool[Pool_CalculateSize(EVENTDISPATCHER_REPORT_DATA_SIZE, EVENTDISPATCHER_REPORT_DATA_POOL_SIZE)];

void EventDispatcher_Initialize()
{
	Queue_Initialize(eventQueue, sizeof(queueElement), EVENTDISPATCHER_QUEUE_SIZE);
	Pool_Initialize(bufferPool, EVENTDISPATCHER_REPORT_DATA_SIZE, EVENTDISPATCHER_REPORT_DATA_POOL_SIZE);
}

void EventDispatcher_Start()
{
}

void EventDispatcher_Dispatch()
{
	while (Queue_IsEmpty(eventQueue) == false)
	{
		queueElement* qe = Queue_Tail(eventQueue);

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

			Pool_ReleaseBlock(bufferPool, qe->publication.data);
		}

		Queue_AdvanceTail(eventQueue);
	}
}

void EventDispatcher_RegisterSubscriber(uint8_t event, eventHandler handler)
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

	queueElement* qe = Queue_Head(eventQueue);

	qe->isNotification = false;
	qe->publication.event = event;
	qe->publication.data = data;

	Queue_AdvanceHead(eventQueue);


	// return new buffer => error if no buffers are available
	return Pool_AllocateBlock(bufferPool);
}

void EventDispatcher_Notify(eventHandler handler)
{
	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		// error
		return;
	}

	queueElement* qe = Queue_Head(eventQueue);

	qe->isNotification = true;
	qe->notification.handler = handler;

	Queue_AdvanceHead(eventQueue);
}
