/*
 * MAX6662.c
 *
 *  Created on: 09/10/2010
 *      Author: coma
 */

#include "MAX6662.h"
#include "../EventSubsystem/EventDispatcher.h"

enum
{
	REGISTER_READ_TEMPERATURE = 0xc1,
	REGISTER_READ_CONFIGURATION = 0xc3,
	REGISTER_READ_T_HYST = 0xc5,
	REGISTER_READ_T_MAX = 0xc7,
	REGISTER_READ_T_LOW = 0xc9,
	REGISTER_READ_T_HIGH = 0xcb,
	REGISTER_WRITE_CONFIGURATION = 0x83,
	REGISTER_WRITE_T_HYST = 0x85,
	REGISTER_WRITE_T_MAX = 0x87,
	REGISTER_WRITE_T_LOW = 0x89,
	REGISTER_WRITE_T_HIGH = 0x8b
};

static void SpiHandler();

static uint8_t data[3];
static int16_t* temp;
static notification_handler handl;

void MAX6662_CreateConfiguration(max6662_configuration* configuration, gpio_pin csPin)
{
	SPI_CreateConfiguration(&configuration->spiConfiguration, 2500000, SPI_DATA_MODE_LEADING_RISING, SPI_DATA_ORDER_MSB_FIRST, csPin);
}

void MAX6662_Initialize(max6662_configuration* configuration)
{
	uint8_t defaultConfiguration[3] = { REGISTER_WRITE_CONFIGURATION, 0x00, 0x00 };
	SPI_Transfer(&configuration->spiConfiguration, defaultConfiguration, defaultConfiguration, 3, NULL);
}

void MAX6662_GetTemperature(max6662_configuration* configuration, int16_t* temperature, notification_handler handler)
{
	temp = temperature;
	handl = handler;

	data[0] = REGISTER_READ_TEMPERATURE;
	data[1] = 0;
	data[2] = 0;
	SPI_Transfer(&configuration->spiConfiguration, data, data, 3, SpiHandler);
}

static void SpiHandler()
{
	*temp = ((uint16_t)data[2] << 8) + data[1];
	EventDispatcher_Notify(handl);
}
