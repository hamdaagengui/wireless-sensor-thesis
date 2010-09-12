/*
 * Adc.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef ADC_H_
#define ADC_H_

#include <Globals.h>

#define PERIPHERAL_ADC_CONSUMPTION_IDLE					3
#define PERIPHERAL_ADC_CONSUMPTION_ACTIVE				550

typedef struct
{
//	peripheralUsageRequest base;
	uint32_t sampleInterval;
} adcConfiguration;

void Adc_Configure(adcConfiguration* configuration);
void Adc_Subscribe();

#endif /* ADC_H_ */
