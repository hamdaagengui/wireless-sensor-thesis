/*
 * Transport.c
 *
 *  Created on: 07/09/2010
 *      Author: coma
 */

#include <Globals.h>
#include "FrameTransceiver.h"

#define NUMBER_OF_CHANNELS											16

typedef struct
{
	blockHandlerCallback callback;

} channel;
channel channels[NUMBER_OF_CHANNELS] = { { 0 } };

static void FrameHandler(uint8_t* data, uint8_t length);

void Transport_Initialize()
{
	FrameTransceiver_Initialize(0, FrameHandler);
}

uint8_t Transport_GetChannel()
{
	for (uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++)
	{
		if (channels[i].callback == NULL)
		{
			return i;
		}
	}

	return -1;
}

bool Transport_RegisterReceiver(uint8_t channel, blockHandlerCallback cb)
{
	if (channels[channel].callback != NULL)
	{
		return false;
	}

	channels[channel].callback = cb;

	return true;
}

void Transport_Send(void* data, uint8_t length, uint8_t channel)
{
	uint8_t d[length + 1];

	d[0] = channel;
	for (uint8_t i = 0; i < length; i++)
	{
		d[i + 1] = ((uint8_t*) data)[i];
	}

	FrameTransceiver_Send(d, length + 1);
}

static void FrameHandler(uint8_t* data, uint8_t length)
{
	if (data[0] >= NUMBER_OF_CHANNELS || channels[data[0]].callback == NULL)
	{
		return;
	}

}
