/*
 * Adc.c
 *
 *  Created on: 13/09/2010
 *      Author: coma
 */

#include "ADC.h"
#include <PeripheralSubsystem/PeripheralSubsystem.h>

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

		eventData = PeripheralDataDistributor_GetBuffer();


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

	uint8_t event = PERIPHERAL_EVENT_ADC_CHANNEL_0 + currentChannel;
	eventData = PeripheralDataDistributor_Publish(event, eventData);


	// set channel = nextChannel
	// start sample

	currentChannel = nextChannel;

	do
	{
		nextChannel++;
		nextChannel &= 0x07;
	} while (activeChannels[nextChannel] == false);
}
