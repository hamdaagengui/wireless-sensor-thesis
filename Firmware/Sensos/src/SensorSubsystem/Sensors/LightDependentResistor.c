/*
 * LightDependentResistor.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "../../HardwareAbstractionLayer/SPI.h"
#include "../../HardwareAbstractionLayer/GPIO.h"
#include "../../EventSubsystem/EventDispatcher.h"

enum
{
	CMD_BLA,
	CMD_BLEH
};

static void TimerTick();
static void ComDone();

// static timerConfiguration timerConfig = { };
static spi_configuration spiConfig = { 1000000, GPIOB3, ComDone };

void LightDependentResistor_Initialize()
{
	// Timer_Subscribe(&timerConfig);
	SPI_Subscribe(&spiConfig);
}

static uint8_t commandBuffer[4] = { CMD_BLA, 2 };
static uint8_t replyBuffer[4];
static void TimerTick()
{
	SPI_Transfer(&spiConfig, commandBuffer, replyBuffer, 4);
}

#define EVENT_LDR 123

static void ComDone()
{
	uint16_t value = (replyBuffer[3] << 8) + replyBuffer[2];
	EventDispatcher_Publish(EVENT_LDR, &value);
}
