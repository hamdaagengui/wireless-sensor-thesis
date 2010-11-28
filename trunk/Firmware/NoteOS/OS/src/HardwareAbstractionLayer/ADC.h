/*
 * Adc.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

// The ADC driver is an example of a "sampling peripheral driver"

#ifndef ADC_H_
#define ADC_H_

#include "../Globals.h"
#include "../EventSubsystem/EventDispatcher.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/ADC.h"
#else
#define ADC_UNIMPLEMENTED
#endif

#ifndef ADC_UNIMPLEMENTED

extern void ADC_Initialize();
extern void ADC_Start();
extern void ADC_CreateConfiguration(adc_configuration* configuration, uint8_t channel, adc_prescaler prescaler, uint16_t* result, notification_handler completed);
extern bool ADC_Convert(adc_configuration* configuration);

#endif

#endif /* ADC_H_ */
