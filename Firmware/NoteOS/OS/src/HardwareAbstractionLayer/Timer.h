/*
 * Timer.h
 *
 *  Created on: 09/10/2010
 *      Author: coma
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "../Globals.h"
#include "../EventSubsystem/EventDispatcher.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/Timer.h"
#else
#error "No timer driver found for the selected processor!"
#endif

typedef enum
{
	TIMER_MODE_ONE_SHOT,
	TIMER_MODE_CONTINUES
} timer_mode;

extern void Timer_Initialize();
extern void Timer_Start();

extern void Timer_CreateConfiguration(timer_configuration* configuration, uint32_t interval, timer_mode mode, completion_handler completed);

extern void Timer_Activate(timer_configuration* configuration);
extern void Timer_Deactivate(timer_configuration* configuration);

#endif /* TIMER_H_ */
