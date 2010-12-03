/*
 * Timer.c
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#include "Timer.h"
#include "../HardwareAbstractionLayer/SystemTimer.h"
#include "../Diagnostics/Diagnostics.h"

static void TickAllTimers();
static void FindAndSetShortestInterval();
static void RemoveTimer(uint8_t index);

static uint32_t localTime = 0;

static timer_configuration* timers[TIMER_MAXIMUM_NUMBER_OF_TIMERS];
static uint8_t timerCount = 0;
static uint16_t interval;

void Timer_CreateConfiguration(timer_configuration* configuration, uint32_t period, timer_mode mode, completion_handler completed)
{
	period /= TIMER_RESOLUTION;

	configuration->period = period;
	configuration->mode = mode;
	configuration->completed = completed;

	Diagnostics_SendEvent(DIAGNOSTICS_TIMER_CONFIGURATION_CREATED);
}

void Timer_Start(timer_configuration* configuration)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (timerCount == TIMER_MAXIMUM_NUMBER_OF_TIMERS)
		{
			Diagnostics_SendEvent(DIAGNOSTICS_TIMER_FAILED_TO_START);
			return;
		}

		interval = SystemTimer_Reset();
		TickAllTimers();

		configuration->timer = configuration->period;
		timers[timerCount++] = configuration;

		FindAndSetShortestInterval();
	}

	SystemTimer_Enable();

	Diagnostics_SendEvent(DIAGNOSTICS_TIMER_STARTED);
}

void Timer_Restart(timer_configuration* configuration)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (uint8_t i = 0; i < timerCount; i++)
		{
			if (timers[i] == configuration)
			{
				interval = SystemTimer_Reset();

				timers[i]->timer = 0xffffffff;

				TickAllTimers();

				timers[i]->timer = timers[i]->period;

				FindAndSetShortestInterval();

				Diagnostics_SendEvent(DIAGNOSTICS_TIMER_RESTARTED);

				return;
			}
		}
	}
}

void Timer_Stop(timer_configuration* configuration)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (uint8_t i = 0; i < timerCount; i++)
		{
			if (timers[i] == configuration)
			{
				RemoveTimer(i);

				interval = SystemTimer_Reset();
				TickAllTimers();

				FindAndSetShortestInterval();

				Diagnostics_SendEvent(DIAGNOSTICS_TIMER_STOPPED);

				return;
			}
		}
	}
}

uint32_t Timer_GetLocalTime()
{
	return (SystemTimer_GetCurrent() + localTime) * TIMER_RESOLUTION;
}

void Timer_Tick()
{
//	Diagnostics_SendEvent(DIAGNOSTICS_TIMER_TICK);
	TickAllTimers();
	FindAndSetShortestInterval();
}

static void TickAllTimers()
{
	for (uint8_t i = 0; i < timerCount; i++)
	{
		timers[i]->timer -= interval;

		if (timers[i]->timer == 0)
		{
			switch (timers[i]->mode)
			{
				case TIMER_MODE_PRECISION_ONE_SHOT:
					Diagnostics_SendEvent(DIAGNOSTICS_TIMER_FIRED_AND_STOPPED);
					timers[i]->completed();
					RemoveTimer(i--);
					break;
				case TIMER_MODE_PRECISION_CONTINUES:
					Diagnostics_SendEvent(DIAGNOSTICS_TIMER_FIRED_AND_CONTINUED);
					timers[i]->completed();
					timers[i]->timer = timers[i]->period;
					break;
				case TIMER_MODE_RELAXED_ONE_SHOT:
					Diagnostics_SendEvent(DIAGNOSTICS_TIMER_FIRED_AND_STOPPED);
					EventDispatcher_Complete(timers[i]->completed);
					RemoveTimer(i--);
					break;
				case TIMER_MODE_RELAXED_CONTINUES:
					Diagnostics_SendEvent(DIAGNOSTICS_TIMER_FIRED_AND_CONTINUED);
					EventDispatcher_Complete(timers[i]->completed);
					timers[i]->timer = timers[i]->period;
					break;
			}
		}
	}

	localTime += interval;
}

static void FindAndSetShortestInterval()
{
	if (timerCount == 0)
	{
		SystemTimer_Disable();
	}
	else
	{
		interval = 0xfff0;

		for (uint8_t i = 0; i < timerCount; i++)
		{
			if (timers[i]->timer < interval)
			{
				interval = timers[i]->timer;
			}
		}

		SystemTimer_SetPeriod(interval);
	}
}

static void RemoveTimer(uint8_t index)
{
	timerCount--;

	if (index < timerCount)
	{
		timers[index] = timers[timerCount];
	}
}
