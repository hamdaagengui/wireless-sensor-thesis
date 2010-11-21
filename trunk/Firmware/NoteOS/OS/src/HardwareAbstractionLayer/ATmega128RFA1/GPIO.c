/*
 * GPIO.c
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../GPIO.h"
#include "../../Globals.h"

enum
{
	OFFSET_PIN,
	OFFSET_DDR,
	OFFSET_PORT
};

typedef volatile uint8_t* physical_port;
static const physical_port ports[] = { &PINB, &PIND, &PINE, &PINF, &PING };

void GPIO_SetupPin(gpio_pin pin, gpio_mode mode)
{
	physical_port p = ports[pin >> 3];
	uint8_t index = pin & 0x07;

	switch (mode)
	{
		case GPIO_MODE_INPUT_FLOATING:
			ClearBit(p[OFFSET_DDR], index);
			ClearBit(p[OFFSET_PORT], index);
			break;
		case GPIO_MODE_INPUT_PULL_UP:
			ClearBit(p[OFFSET_DDR], index);
			SetBit(p[OFFSET_PORT], index);
			break;
		case GPIO_MODE_OUTPUT_LOW:
			SetBit(p[OFFSET_DDR], index);
			ClearBit(p[OFFSET_PORT], index);
			break;
		case GPIO_MODE_OUTPUT_HIGH:
			SetBit(p[OFFSET_DDR], index);
			SetBit(p[OFFSET_PORT], index);
			break;
	}
}

bool GPIO_ReadPin(gpio_pin pin)
{
	physical_port p = ports[pin >> 3];
	uint8_t index = pin & 0x07;

	return ReadBit(p[OFFSET_PIN], index);
}

void GPIO_WritePin(gpio_pin pin, bool value)
{
	physical_port p = ports[pin >> 3];
	uint8_t index = pin & 0x07;

	if (value)
	{
		SetBit(p[OFFSET_PORT], index);
	}
	else
	{
		ClearBit(p[OFFSET_PORT], index);
	}
}

void GPIO_ClearPin(gpio_pin pin)
{
	physical_port p = ports[pin >> 3];
	uint8_t index = pin & 0x07;

	ClearBit(p[OFFSET_PORT], index);
}

void GPIO_SetPin(gpio_pin pin)
{
	physical_port p = ports[pin >> 3];
	uint8_t index = pin & 0x07;

	SetBit(p[OFFSET_PORT], index);
}

void GPIO_TogglePin(gpio_pin pin)
{
	physical_port p = ports[pin >> 3];
	uint8_t index = pin & 0x07;

	ToggleBit(p[OFFSET_PORT], index);
}

void GPIO_SetupPort(gpio_port pin, gpio_mode mode)
{
	physical_port p = ports[pin >> 3];

	switch (mode)
	{
		case GPIO_MODE_INPUT_FLOATING:
			p[OFFSET_DDR] = 0x00;
			p[OFFSET_PORT] = 0x00;
			break;
		case GPIO_MODE_INPUT_PULL_UP:
			p[OFFSET_DDR] = 0x00;
			p[OFFSET_PORT] = 0xff;
			break;
		case GPIO_MODE_OUTPUT_LOW:
			p[OFFSET_DDR] = 0xff;
			p[OFFSET_PORT] = 0x00;
			break;
		case GPIO_MODE_OUTPUT_HIGH:
			p[OFFSET_DDR] = 0xff;
			p[OFFSET_PORT] = 0xff;
			break;
	}
}

uint8_t GPIO_ReadPort(gpio_port pin)
{
	physical_port p = ports[pin >> 3];

	return p[OFFSET_PIN];
}

void GPIO_WritePort(gpio_port pin, uint8_t value)
{
	physical_port p = ports[pin >> 3];

	p[OFFSET_PORT] = value;
}

#endif
