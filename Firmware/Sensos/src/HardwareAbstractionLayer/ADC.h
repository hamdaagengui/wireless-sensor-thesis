/*
 * Adc.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

// The ADC driver is an example off a "sampling peripheral driver"

#ifndef ADC_H_
#define ADC_H_

#include <Globals.h>
#include "../EventSubsystem/EventDispatcher.h"
#include "../DefaultConfiguration.h"

// TODO Really belongs to the device specific peripheral  driver
#define PERIPHERAL_ADC_CONSUMPTION_IDLE					3
#define PERIPHERAL_ADC_CONSUMPTION_ACTIVE				550

typedef struct
{
	uint32_t interval;
	uint8_t channel;
	completionHandler completed;
} adcConfiguration;

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/ADC.h"
#else
#error "No SPI driver found for the selected processor!"
#endif

extern void ADC_Initialize();
extern void ADC_Subscribe(adcConfiguration* configuration);
extern void ADC_Start();

#endif /* ADC_H_ */
