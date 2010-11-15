/*
 * NetworkTimer.c
 *
 *  Created on: 11/10/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

//#include <avr/power.h>
#include "../NetworkTimer.h"
#include "../../NetworkSubsystem/Network.h"

#define PRESCALER_VALUE																							8

void NetworkTimer_Initialize()
{
	ASSR = (1 << AS2);

	TCCR2A = (1 << WGM21);
	TCCR2B = (1 << CS21);

	TIFR2 = (1 << OCF2B) | (1 << OCF2A) | (1 << TOV2);
	TIMSK2 = (1 << OCIE2A);
}

void NetworkTimer_SetTimerPeriod(uint16_t value)
{
	value += PRESCALER_VALUE; // for rounding
	OCR2A = value / PRESCALER_VALUE;
}

void NetworkTimer_SetTimerValue(uint16_t value)
{
	GTCCR |= (1 << PSRASY); // reset prescaler
	TCNT2 = value / PRESCALER_VALUE;
}

// Internals

ISR(TIMER2_COMPA_vect)
{
	Network_TimerEvent();
}

#endif
