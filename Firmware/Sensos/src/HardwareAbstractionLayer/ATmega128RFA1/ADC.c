/*
 * Adc.c
 *
 *  Created on: 13/09/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../ADC.h"
#include "../../Collections/Queue.h"
#include "../../EventSubsystem/EventDispatcher.h"

static bool enable = false;

static uint8_t currentChannel = 0;
static uint8_t nextChannel = 0;
static bool activeChannels[8] = { false, false, false, false, false, false, false, false };

static uint16_t* eventData = NULL;

void ADC_Initialize()
{
	// nothing to setup that can't be done at compile time
}

void ADC_Subscribe(adcConfiguration* configuration)
{
	enable = true;


	// save configuration
}

void ADC_Start()
{
	if (enable)
	{
		// calculate timer settings

		eventData = EventDispatcher_RegisterPublisher(0);


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
	eventData = EventDispatcher_Publish(event, eventData);


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
