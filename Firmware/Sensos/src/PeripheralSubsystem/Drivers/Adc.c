/*
 * Adc.c
 *
 *  Created on: 13/09/2010
 *      Author: coma
 */

#include "Adc.h"
#include <PeripheralSubsystem/PeripheralSubsystem.h>

uint16_t* buffer;

void Adc_Initialize()
{
	buffer = PeripheralDataDistributor_RegisterPublisher(PERIPHERAL_ADC);
}

void Adc_Start()
{

}

ISR(ADC_vect)
{
	*buffer = ADC;
	buffer = PeripheralDataDistributor_Publish(PERIPHERAL_ADC, buffer);
}
