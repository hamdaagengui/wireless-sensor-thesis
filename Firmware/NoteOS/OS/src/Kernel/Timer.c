/*
 * SPI.c
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#include "Timer.h"
#include "../HardwareAbstractionLayer/SystemTimer.h"

static void UpdateTimers();
static void CalculatePeriod();
static void RemoveTimer(uint8_t index);

static timer_configuration* timers[TIMER_MAXIMUM_NUMBER_OF_TIMERS];
static uint8_t timerCount;
static uint16_t interval;

void Timer_CreateConfiguration(timer_configuration* configuration, uint32_t period, timer_mode mode, completion_handler completed)
{
	interval /= TIMER_RESOLUTION;

	configuration->period = period;
	configuration->timer = period;
	configuration->mode = mode;
	configuration->completed = completed;
}

void Timer_Start(timer_configuration* configuration)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (timerCount == TIMER_MAXIMUM_NUMBER_OF_TIMERS)
		{
			return;
		}

		interval = SystemTimer_Reset();
		UpdateTimers();

		timers[timerCount] = configuration;
		timerCount++;

		CalculatePeriod();
	}

	SystemTimer_Enable();
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
				UpdateTimers();

				timers[i]->timer = timers[i]->period;

				CalculatePeriod();

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
				interval = SystemTimer_Reset();
				UpdateTimers();

				RemoveTimer(i);

				CalculatePeriod();

				return;
			}
		}
	}
}

void Timer_Tick()
{
	UpdateTimers();
	CalculatePeriod();
}

static void UpdateTimers()
{
	for (uint8_t i = 0; i < timerCount; i++)
	{
		timers[i]->timer -= interval;
		if (timers[i]->timer == 0)
		{
			switch (timers[i]->mode)
			{
				case TIMER_MODE_PRECISION_ONE_SHOT:
					timers[i]->completed();
					RemoveTimer(i--);
					break;
				case TIMER_MODE_PRECISION_CONTINUES:
					timers[i]->completed();
					timers[i]->timer = timers[i]->period;
					break;
				case TIMER_MODE_RELAXED_ONE_SHOT:
					EventDispatcher_Notify(timers[i]->completed);
					RemoveTimer(i--);
					break;
				case TIMER_MODE_RELAXED_CONTINUES:
					EventDispatcher_Notify(timers[i]->completed);
					timers[i]->timer = timers[i]->period;
					break;
			}
		}
	}
}

static void CalculatePeriod()
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
	if (index < (timerCount - 1))
	{
		timers[index] = timers[timerCount - 1];
	}
	timerCount--;
}
