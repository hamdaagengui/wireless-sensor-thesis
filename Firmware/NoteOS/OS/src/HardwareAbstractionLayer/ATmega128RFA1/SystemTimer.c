/*
 * SystemTimer.c
 *
 *  Created on: 16/11/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../SystemTimer.h"
#include "../../Kernel/Timer.h"

extern void Timer_Tick();

enum
{
	PRESCALER_DISABLE,
	PRESCALER_1,
	PRESCALER_8,
	PRESCALER_64,
	PRESCALER_256,
	PRESCALER_1024
};

void SystemTimer_Initialize()
{
	TCCR5A = 0x00;
	TCCR5B = (1 << WGM52) | PRESCALER_1024;
	TCCR5C = 0x00;
	TIFR5 = 0x0f;
	TIMSK5 = 0x00;
}

void SystemTimer_Enable()
{
	TIMSK5 |= 1 << OCIE5A;
}

void SystemTimer_Disable()
{
	TIMSK5 &= ~(1 << OCIE5A);
}

void SystemTimer_SetPeriod(uint16_t period)
{
	OCR5A = period;
	TIFR5 |= 1 << OCF5A;
}

uint16_t SystemTimer_Reset()
{
	uint16_t timer = TCNT5;

	TCNT5 = 0;

	TIFR5 |= 1 << OCF5A;

	return timer;
}

ISR(TIMER5_COMPA_vect)
{
	Timer_Tick();
}

#endif
