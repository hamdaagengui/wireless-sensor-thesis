/*
 * StopWatch.c
 *
 *  Created on: 17/11/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../StopWatch.h"

static uint16_t overflows;

void StopWatch_Initialize()
{
	// setup timer 4: prescaler = 1
	// enable overflow interrupt
}

void StopWatch_Start()
{
	overflows = 0;
	// clear TCNT4
	// start timer
}

void StopWatch_Restart()
{
	// stop timer
	overflows = 0;
	// clear TCNT4
	// start timer
}

uint32_t StopWatch_Stop()
{
	// stop timer

	uint32_t ticks = overflows;
	ticks <<= 16;
	ticks += TCNT4;

	return ticks;
}

void OverflowInterrupt()
{
	overflows++;
}

#endif
