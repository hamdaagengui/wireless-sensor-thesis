/*
 * EventDispatcher.c
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#include "EventDispatcher.h"
#include "../Collections/Queue.h"
#include "../MemorySubsystem/MemoryManager.h"

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

void EventDispatcher_Start()
{
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
				break;

			case TYPE_PROCESSING:
				qe->processing.handler(qe->processing.data, qe->processing.length);

				if (qe->processing.data != NULL)
				{
					MemoryManager_ReleaseAnyBlock(qe->processing.data);
				}
				break;

			case TYPE_PUBLICATION:
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
					MemoryManager_ReleaseAnyBlock(qe->publication.data);
				}
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


	// full => error
}

void EventDispatcher_Notify(completion_handler handler)
{
	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		// error
		return;
	}

	queue_element* qe = Queue_Head(eventQueue);

	qe->type = TYPE_NOTIFICATION;
	qe->notification.handler = handler;

	Queue_AdvanceHead(eventQueue);
}

void EventDispatcher_Process(block_handler handler, void* data, uint8_t length)
{
	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		// error
		return;
	}

#ifdef DEBUG
	if (event >= EVENTDISPATCHER_HIGHEST_EVENT_ID)
	{

	}
#endif

	queue_element* qe = Queue_Head(eventQueue);

	qe->type = TYPE_PROCESSING;
	qe->processing.handler = handler;
	qe->processing.data = data;
	qe->processing.length = length;

	Queue_AdvanceHead(eventQueue);
}

void EventDispatcher_Publish(uint8_t event, void* data)
{
	if (Queue_IsFull(eventQueue)) // queue full ?
	{
		// error
		return;
	}

#ifdef DEBUG
	if (event >= EVENTDISPATCHER_HIGHEST_EVENT_ID)
	{

	}
#endif

	queue_element* qe = Queue_Head(eventQueue);

	qe->type = TYPE_PUBLICATION;
	qe->publication.event = event;
	qe->publication.data = data;

	Queue_AdvanceHead(eventQueue);
}
