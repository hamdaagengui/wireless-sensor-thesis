/*
 * PowerManager.c
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include <avr/sleep.h>
#include "../PowerManager.h"
#include "../../Diagnostics/Diagnostics.h"

static uint8_t resourceRequests[NUMBER_OF_PROCESSOR_RESOURCES];

void PowerManager_Initialize()
{

}

void PowerManager_RequestResource(processor_resource resource)
{
	if (resourceRequests[resource] < 255)
	{
		resourceRequests[resource]++;
	}
	else
	{
		Diagnostics_SendEvent(DIAGNOSTICS_POWER_MANAGER_RESOURCE_REQUEST_OVERFLOW);
	}
}

void PowerManager_ReleaseResource(processor_resource resource)
{
	if (resourceRequests[resource] > 0)
	{
		resourceRequests[resource]--;
	}
	else
	{
		Diagnostics_SendEvent(DIAGNOSTICS_POWER_MANAGER_RESOURCE_RELEASE_OVERFLOW);
	}
}

void PowerManager_PowerDown()
{
	uint8_t sleepMode = SLEEP_MODE_PWR_DOWN; // choose most aggressive mode as default and degrade it based on resource usage
	if (resourceRequests[PROCESSOR_RESOURCE_ADC_CLOCK])
	{
		//...
	}

	set_sleep_mode(sleepMode);
	cli();
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
}

#endif
