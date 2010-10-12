/*
 * NetworkTimer.c
 *
 *  Created on: 11/10/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include <avr/power.h>
#include "../NetworkTimer.h"

static notification_handler hnd;

void NetworkTimer_Initialize(notification_handler handler)
{
	hnd = handler;
}

void NetworkTimer_SetTimerValue(uint16_t value)
{

}

void NetworkTimer_SetTimerPeriod(uint16_t value)
{

}

// Internals

ISR(TIMER2_COMPA_vect)
{
	hnd();
}

#endif
