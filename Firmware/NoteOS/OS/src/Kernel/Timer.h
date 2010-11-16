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
#include "../HardwareAbstractionLayer/SystemTimer.h"

#define TIMER_RESOLUTION																						(1000000UL / (F_CPU / SYSTEM_TIMER_PRESCALER))

typedef enum
{
	TIMER_MODE_PRECISION_ONE_SHOT,
	TIMER_MODE_PRECISION_CONTINUES,
	TIMER_MODE_RELAXED_ONE_SHOT,
	TIMER_MODE_RELAXED_CONTINUES
} timer_mode;

typedef struct
{
	uint32_t interval;
	uint32_t timer;
	timer_mode mode;
	completion_handler completed;
} timer_configuration;

extern void Timer_Initialize();
extern void Timer_CreateConfiguration(timer_configuration* configuration, uint32_t interval, timer_mode mode, completion_handler completed);
extern void Timer_Start(timer_configuration* configuration);
extern void Timer_Restart(timer_configuration* configuration);
extern void Timer_Stop(timer_configuration* configuration);

#endif /* TIMER_H_ */
