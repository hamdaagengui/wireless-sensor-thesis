/*
 * PeripheralDataDistributor.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "PeripheralDataDistributor.h"
#include <DefaultConfiguration.h>

static peripheralEventHandler subscribers[PDD_MAX_EVENTS][PDD_MAX_SUBSCRIBERS];

typedef struct
{
	uint8_t event;
	void* data;
} queueElement;
static queueElement eventQueue[PDD_QUEUE_SIZE];
static uint8_t queueIn = 0;
static uint8_t queueOut = 0;
static volatile uint8_t queueFree = PDD_QUEUE_SIZE;

void PeripheralDataDistributor_Initialize()
{
	for (uint8_t i = 0; i < PDD_MAX_EVENTS; i++)
	{
		for (uint8_t j = 0; j < PDD_MAX_SUBSCRIBERS; j++)
		{
			subscribers[i][j] = NULL;
		}
	}
}

void PeripheralDataDistributor_Start()
{

}

void PeripheralDataDistributor_ProcessEvents()
{
	while (queueFree < PDD_QUEUE_SIZE)
	{
		queueElement* qe = &eventQueue[queueOut];

		for (uint8_t i = 0; i < PDD_MAX_SUBSCRIBERS; i++)
		{
			if (subscribers[qe->event][i] != NULL)
			{
				subscribers[qe->event][i](qe->data);
			}
			else // no more subscribers
			{
				break;
			}
		}

		// Remove element from queue
		if (++queueOut >= PDD_QUEUE_SIZE)
		{
			queueOut = 0;
		}
		atomic(queueFree++);
	}
}

void PeripheralDataDistributor_RegisterSubscriber(uint8_t event, peripheralEventHandler handler)
{
	for (uint8_t i = 0; i < PDD_MAX_SUBSCRIBERS; i++)
	{
		if (subscribers[event][i] == NULL)
		{
			subscribers[event][i] = handler;
			return;
		}
	}


	// full => error
}

void* PeripheralDataDistributor_GetBuffer()
{
	// return buffer => error if no buffers are available
	return NULL;
}

void* PeripheralDataDistributor_RegisterPublisher(uint8_t id)
{
	// return buffer => error if no buffers are available
	return NULL;
}

void* PeripheralDataDistributor_Publish(uint8_t event, void* data)
{
	if (queueFree == 0) // queue full ?
	{
		// error
		return data; // just return the same buffer since it will not be passed on to event handlers
	}

	eventQueue[queueIn].event = event;
	eventQueue[queueIn].data = data;

	if (++queueIn >= PDD_QUEUE_SIZE)
	{
		queueIn = 0;
	}
	queueFree--;


	// return new buffer => error if no buffers are available
	return NULL;
}
