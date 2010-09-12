/*
 * PeripheralSubsystem.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef PERIPHERALSUBSYSTEM_H_
#define PERIPHERALSUBSYSTEM_H_

#include <Globals.h>
#include "PeripheralDataDistributor.h"
#include "Drivers/Adc.h"

enum
{
	PERIPHERAL_ADC,
	PERIPHERAL_SPI,
	PERIPHERAL_IIC
};

enum
{
	EVENT_ADC_CHANNEL_0,
	EVENT_ADC_CHANNEL_1,
	EVENT_ADC_CHANNEL_2,
	EVENT_ADC_CHANNEL_3,
	EVENT_ADC_CHANNEL_4,
	EVENT_ADC_CHANNEL_5,
	EVENT_ADC_CHANNEL_6,
	EVENT_ADC_CHANNEL_7,
};

typedef struct
{
	uint8_t peripheralId;
	peripheralEventHandler handler;
} peripheralUsageRequest;

#endif /* PERIPHERALSUBSYSTEM_H_ */
