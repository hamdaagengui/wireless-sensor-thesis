/*
 * NetworkTimer.c
 *
 *  Created on: 11/10/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../NetworkTimer.h"
#include "../../NetworkSubsystem/Network.h"

extern void Network_TimerEvent();

void NetworkTimer_Initialize()
{
	ASSR = (1 << AS2);

	TCCR2A = 0;
	TCCR2B = (1 << CS20); // prescaler 1:1

	TIFR2 = (1 << TOV2);
	TIMSK2 = (1 << TOIE2);
}

void NetworkTimer_SetTimerPeriod(uint16_t value)
{
	//	value += PRESCALER_VALUE; // for rounding
	//	OCR2A = value / PRESCALER_VALUE;
}

void NetworkTimer_SetTimerValue(uint16_t value)
{
	//	GTCCR |= (1 << PSRASY); // reset prescaler
	TCNT2 = value;
}

ISR(TIMER2_OVF_vect)
{
	Network_TimerEvent();
}

#endif
