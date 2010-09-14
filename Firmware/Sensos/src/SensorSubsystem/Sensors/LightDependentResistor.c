/*
 * LightDependentResistor.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include <PeripheralSubsystem/PeripheralSubsystem.h>

static void EventHandler(uint8_t event, void* arguments);

void LightDependentResistor_Initialize()
{
	adcConfiguration c;
	//	c.base.peripheralId = PERIPHERAL_ADC;
	c.interval = 1000;
	//	Adc_Configure(&c);
}

static void EventHandler(uint8_t event, void* arguments)
{

}

/*

 - Register call back
 - Configure

 */
