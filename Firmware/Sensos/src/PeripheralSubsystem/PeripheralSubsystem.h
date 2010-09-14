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
	PERIPHERAL_EVENT_ADC_CHANNEL_0,
	PERIPHERAL_EVENT_ADC_CHANNEL_1,
	PERIPHERAL_EVENT_ADC_CHANNEL_2,
	PERIPHERAL_EVENT_ADC_CHANNEL_3,
	PERIPHERAL_EVENT_ADC_CHANNEL_4,
	PERIPHERAL_EVENT_ADC_CHANNEL_5,
	PERIPHERAL_EVENT_ADC_CHANNEL_6,
	PERIPHERAL_EVENT_ADC_CHANNEL_7,
	PERIPHERAL_EVENT_SPI,
	PERIPHERAL_EVENT_IIC_TRANSACTION_COMPLETED,
	PERIPHERAL_EVENT_IIC_TRANSACTION_FAILED,
	PERIPHERAL_EVENT_COUNT
};

typedef struct
{
	uint8_t peripheralId;
	peripheralEventHandler handler;
} peripheralUsageRequest;

#endif /* PERIPHERALSUBSYSTEM_H_ */
