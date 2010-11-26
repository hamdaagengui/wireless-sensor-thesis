/*
 * MAX6662.c
 *
 *  Created on: 09/10/2010
 *      Author: coma
 */

#include "../HardwareAbstractionLayer/SPI.h"
#include "../HardwareAbstractionLayer/GPIO.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../EventSubsystem/Timer.h"
#include "../MemorySubsystem/MemoryManager.h"

static void TimerTick();
static void ComDone();

static timer_configuration timerConfig;
static spi_configuration spiConfig;
static uint8_t* data;

void MAX6662_Initialize(uint16_t interval)
{
	uint32_t intervalMillisecond = (uint32_t) interval * 1000L;
	Timer_CreateConfiguration(&timerConfig, intervalMillisecond, TIMER_MODE_PRECISION_CONTINUES, TimerTick);
	SPI_CreateConfiguration(&spiConfig, 2500000, SPI_DATA_MODE_LEADING_RISING, SPI_DATA_ORDER_LSB_FIRST, GPIOD2, ComDone);
	data = MemoryManager_Allocate(2);
}

enum
{
	REGISTER_TEMPERATURE_READ = 0xc1,
	REGISTER_CONFIGURATION_READ = 0xc4
};
static uint8_t commandBuffer[3] = { REGISTER_TEMPERATURE_READ };
static uint8_t replyBuffer[3];

static void TimerTick()
{
	SPI_Transfer(&spiConfig, commandBuffer, replyBuffer, 3);
}

static void ComDone()
{
	*((int16_t*) data) = ((replyBuffer[2] << 8) + replyBuffer[1]) & 0xfff8;
	//	EventDispatcher_Publish(EVENT_SENSOR_MAX6662, data);
	data = MemoryManager_Allocate(2);
}
