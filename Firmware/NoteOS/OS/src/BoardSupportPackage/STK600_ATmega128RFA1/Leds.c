/*
 * Leds.c
 *
 *  Created on: 19/10/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../../HardwareAbstractionLayer/GPIO.h"

void Leds_Initialize()
{
	GPIO_SetupPin(GPIOE2, GPIO_MODE_OUTPUT_HIGH);
	GPIO_SetupPin(GPIOE3, GPIO_MODE_OUTPUT_HIGH);
	GPIO_SetupPin(GPIOE4, GPIO_MODE_OUTPUT_HIGH);
}

void Leds_GreenOn()
{
	//GPIO_ClearPin(GPIOE2);
}

void Leds_GreenOff()
{
//	GPIO_SetPin(GPIOE2);
}

void Leds_GreenToggle()
{
//	GPIO_TogglePin(GPIOE2);
}

void Leds_YellowOn()
{
//	GPIO_ClearPin(GPIOE3);
}

void Leds_YellowOff()
{
//	GPIO_SetPin(GPIOE3);
}

void Leds_YellowToggle()
{
//	GPIO_TogglePin(GPIOE3);
}

void Leds_RedOn()
{
//	GPIO_ClearPin(GPIOE4);
}

void Leds_RedOff()
{
//	GPIO_SetPin(GPIOE4);
}

void Leds_RedToggle()
{
//	GPIO_TogglePin(GPIOE4);
}

#endif
