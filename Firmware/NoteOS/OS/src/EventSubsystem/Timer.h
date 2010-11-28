/*
 * Timer.h
 *
 *  Created on: 09/10/2010
 *      Author: coma
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "../Globals.h"
#include "EventDispatcher.h"
#include "../HardwareAbstractionLayer/SystemTimer.h"

/**
 * Specifies the resolution of the timer used for timers. Depends on processor frequency and the timers available on the selected processor.
 */
#define TIMER_RESOLUTION																						(1000000UL / (F_CPU / SYSTEM_TIMER_PRESCALER))

typedef enum
{
	TIMER_MODE_PRECISION_ONE_SHOT, //!< TIMER_MODE_PRECISION_ONE_SHOT
	TIMER_MODE_PRECISION_CONTINUES,//!< TIMER_MODE_PRECISION_CONTINUES
	TIMER_MODE_RELAXED_ONE_SHOT, //!< TIMER_MODE_RELAXED_ONE_SHOT
	TIMER_MODE_RELAXED_CONTINUES //!< TIMER_MODE_RELAXED_CONTINUES
} timer_mode;

typedef struct
{
	uint32_t period;
	uint32_t timer;
	timer_mode mode;
	notification_handler completed;
} timer_configuration;

/**
 * Creates a timer by initializing the timer configuration object.
 * @param configuration The timer configuration object.
 * @param period The period of the timer in units of one microsecond. Use the constant TIMER_RESOLUTION to acquire the actual resolution of the underlying timer.
 * @param mode
 * @param completed
 */
extern void Timer_CreateConfiguration(timer_configuration* configuration, uint32_t period, timer_mode mode, notification_handler completed);

/**
 *
 * @param configuration
 */
extern void Timer_Start(timer_configuration* configuration);

/**
 *
 * @param configuration
 */
extern void Timer_Restart(timer_configuration* configuration);

/**
 *
 * @param configuration
 */
extern void Timer_Stop(timer_configuration* configuration);

#endif /* TIMER_H_ */
