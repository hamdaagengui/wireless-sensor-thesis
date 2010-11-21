/*
 * Leds.c
 *
 *  Created on: 19/10/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../../Globals.h"

void Leds_Initialize()
{

}

void Leds_GreenOn()
{
	ClearBit(PORTE, 2);
}

void Leds_GreenOff()
{
	SetBit(PORTE, 2);
}

void Leds_GreenToggle()
{
	ToggleBit(PORTE, 2);
}

void Leds_YellowOn()
{
	ClearBit(PORTE, 3);
}

void Leds_YellowOff()
{
	SetBit(PORTE, 3);
}

void Leds_YellowToggle()
{
	ToggleBit(PORTE, 3);
}

void Leds_RedOn()
{
	ClearBit(PORTE, 4);
}

void Leds_RedOff()
{
	SetBit(PORTE, 4);
}

void Leds_RedToggle()
{
	ToggleBit(PORTE, 4);
}

#endif
