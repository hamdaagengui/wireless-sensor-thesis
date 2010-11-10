/*
 * Adc.c
 *
 *  Created on: 13/09/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../ADC.h"
#include "../../MemorySubsystem/MemoryManager.h"
#include "../../EventSubsystem/EventDispatcher.h"

static bool enable;

static uint8_t currentChannel;
static uint8_t nextChannel;
static bool activeChannels[8];

static uint16_t* eventData;

void ADC_Initialize()
{
	// nothing to setup that can't be done at compile time
}

void ADC_Start()
{
	if (enable)
	{
		// calculate timer settings

		eventData = MemoryManager_AllocateSensorBlock();


		// kick off sampling
	}
	else
	{
		// power down peripheral
	}
}

ISR( ADC_vect)
{
	*eventData = ADC;


	// TODO Wtf to do about event id allocation???
	uint8_t event = 0;// PERIPHERAL_EVENT_ADC_CHANNEL_0 + currentChannel;
	EventDispatcher_Publish(event, eventData);
	eventData = MemoryManager_AllocateSensorBlock();


	// set channel = nextChannel
	// start sample

	currentChannel = nextChannel;

	do
	{
		nextChannel++;
		nextChannel &= 0x07;
	} while (activeChannels[nextChannel] == false);
}

#endif
