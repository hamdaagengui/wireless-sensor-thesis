/*
 * Adc.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef ADC_H_
#define ADC_H_

#include "../../Globals.h"

#define PERIPHERAL_ADC_CONSUMPTION_IDLE					3
#define PERIPHERAL_ADC_CONSUMPTION_ACTIVE				550

typedef struct
{
	uint8_t channel;
	uint16_t value;
} adcEvent;

typedef struct
{
	uint32_t period;
	uint8_t channel;
	void (*handler)(adcEvent* event);
} adc_configuration;

extern void ADC_Initialize();
extern void ADC_Subscribe(adc_configuration* configuration);
extern void ADC_Start();

#endif /* ADC_H_ */
