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

#define PERIPHERAL_ADC_CONSUMPTION_IDLE					3
#define PERIPHERAL_ADC_CONSUMPTION_ACTIVE				550

typedef struct
{
	uint8_t channel;
	uint16_t value;
} adcEvent;

typedef struct
{
	uint32_t interval;
	uint8_t channel;
	void (*handler)(adcEvent* event);
} adcConfiguration;

extern void ADC_Initialize();
extern void ADC_Subscribe(adcConfiguration* configuration);
extern void ADC_Start();

#endif /* ADC_H_ */
