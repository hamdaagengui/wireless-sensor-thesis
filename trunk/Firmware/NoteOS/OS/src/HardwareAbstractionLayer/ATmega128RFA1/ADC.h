/*
 * Adc.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "../../Globals.h"

#define PERIPHERAL_ADC_CONSUMPTION_IDLE					3
#define PERIPHERAL_ADC_CONSUMPTION_ACTIVE				550

typedef enum
{
	ADC_PRESCALER_2 = 1,
	ADC_PRESCALER_4,
	ADC_PRESCALER_8,
	ADC_PRESCALER_16,
	ADC_PRESCALER_32,
	ADC_PRESCALER_64,
	ADC_PRESCALER_128
} adc_prescaler;

//enum
//{
//	ADC_REFERENCE_AREF,
//	ADC_REFERENCE_AVDD,
//	ADC_REFERENCE_1V5,
//	ADC_REFERENCE_1V6
//} adc_reference;

typedef struct
{
	uint8_t admux;
	uint8_t adcsra;
	uint8_t adcsrb;
	uint8_t adcsrc;
	uint16_t* result;
	completion_handler completed;
} adc_configuration;
