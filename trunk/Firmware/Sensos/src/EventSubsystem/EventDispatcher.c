/*
 * EventDispatcher.c
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#include "EventDispatcher.h"

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

static queueElement eventQueue[EVENTDISPATCHER_QUEUE_SIZE];
static uint8_t queueIn = 0;
static uint8_t queueOut = 0;
static volatile uint8_t queueFree = EVENTDISPATCHER_QUEUE_SIZE;

static void BufferPool_Initialize();
static void* BufferPool_AllocateBuffer();
static void BufferPool_ReleaseBuffer(void* buffer);

void EventDispatcher_Initialize()
{
	for (uint8_t i = 0; i < EVENTDISPATCHER_HIGHEST_EVENT_ID; i++)
	{
		for (uint8_t j = 0; j < EVENTDISPATCHER_MAXIMUM_NUMBER_OF_SUBSCRIBERS; j++)
		{
			subscribers[i][j] = NULL;
		}
	}

	BufferPool_Initialize();
}

void EventDispatcher_Start()
{
}

void EventDispatcher_ProcessEvents()
{
	while (queueFree < EVENTDISPATCHER_QUEUE_SIZE)
	{
		queueElement* qe = &eventQueue[queueOut];
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

			BufferPool_ReleaseBuffer(qe->publication.data);
		}

		// Remove element from queue
		if (++queueOut >= EVENTDISPATCHER_QUEUE_SIZE)
		{
			queueOut = 0;
		}
		atomic(queueFree++);
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
	return BufferPool_AllocateBuffer();
}

void* EventDispatcher_Publish(uint8_t event, void* data)
{
	if (queueFree == 0) // queue full ?
	{
		// error
		return data; // just return the same buffer since it will not be passed on to event handlers
	}

#ifdef DEBUG
	if (event >= EVENTDISPATCHER_HIGHEST_EVENT_ID)
	{

	}
#endif

	eventQueue[queueIn].isNotification = false;
	eventQueue[queueIn].publication.event = event;
	eventQueue[queueIn].publication.data = data;

	if (++queueIn >= EVENTDISPATCHER_QUEUE_SIZE)
	{
		queueIn = 0;
	}
	atomic(queueFree--);


	// return new buffer => error if no buffers are available
	return BufferPool_AllocateBuffer();
}

void EventDispatcher_Notify(eventHandler handler)
{
	if (queueFree == 0) // queue full ?
	{
		// error
		return;
	}

	eventQueue[queueIn].isNotification = true;
	eventQueue[queueIn].notification.handler = handler;

	if (++queueIn >= EVENTDISPATCHER_QUEUE_SIZE)
	{
		queueIn = 0;
	}
	atomic(queueFree--);
}

// Internals

typedef struct
{
	bool used;
	uint8_t store[EVENTDISPATCHER_REPORT_DATA_SIZE];
} eventBuffer;
static eventBuffer bufferPool[EVENTDISPATCHER_REPORT_DATA_POOL_SIZE];
static uint8_t lastAllocatedBuffer = 0;

static void BufferPool_Initialize()
{
	for (uint8_t i = 0; i < EVENTDISPATCHER_REPORT_DATA_POOL_SIZE; i++)
	{
		bufferPool[i].used = false;
	}
}

static void* BufferPool_AllocateBuffer()
{
	uint8_t count = EVENTDISPATCHER_REPORT_DATA_POOL_SIZE;
	while (count--)
	{

	}

	return NULL;
}

static void BufferPool_ReleaseBuffer(void* buffer)
{

}
