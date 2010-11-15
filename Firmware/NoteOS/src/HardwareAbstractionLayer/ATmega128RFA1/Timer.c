/*
 * SPI.c
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include <avr/power.h>
#include "../Timer.h"
#include "../../Collections/Queue.h"
#include "../../EventSubsystem/EventDispatcher.h"

static bool enabled;

//typedef struct
//{
//	spi_configuration* configuration;
//	uint8_t* output;
//	uint8_t* input;
//	uint8_t length;
//} transfer_command;
//static uint8_t transferQueue[Queue_CalculateSize(sizeof(transfer_command), Timer_TRANSFER_QUEUE_SIZE)];
//
//static transfer_command* currentTransfer;
//static uint8_t* currentOutput;
//static uint8_t* currentInput;
//static uint8_t remainingBytes;

void Timer_Initialize()
{
}

void Timer_Subscribe(timer_configuration* configuration)
{
	enabled = true;
}

void Timer_Start()
{
	if (enabled)
	{
		//power_spi_enable();


		// basic initialization - only that which is not set by each separate transfers configuration.
		//		Queue_Initialize(transferQueue, sizeof(transfer_command), Timer_TRANSFER_QUEUE_SIZE);

		PORTB |= (1 << 2) | (1 << 1);

		uint8_t dummy = SPSR; // make sure interrupt flag is cleared
		dummy = SPDR;
	}
	else
	{
		//power_spi_disable();
	}
}

void Timer_CreateConfiguration(timer_configuration* configuration, uint32_t interval, timer_mode mode, completion_handler completed)
{
	//	configuration->spcr = (1 << SPIE) | (1 << SPE) | (1 << MSTR);
	//	configuration->spsr = 0;
	//
	//	switch (order)
	//	{
	//		case Timer_DATA_ORDER_LSB_FIRST:
	//			configuration->spcr |= (1 << DORD);
	//			break;
	//
	//		case Timer_DATA_ORDER_MSB_FIRST:
	//			break;
	//	}
	//
	//	switch (mode)
	//	{
	//		case Timer_DATA_MODE_LEADING_RISING:
	//			break;
	//
	//		case Timer_DATA_MODE_LEADING_FALLING:
	//			configuration->spcr |= (1 << CPOL);
	//			break;
	//
	//		case Timer_DATA_MODE_TRAILING_FALLING:
	//			configuration->spcr |= (1 << CPHA);
	//			break;
	//
	//		case Timer_DATA_MODE_TRAILING_RISING:
	//			configuration->spcr |= (1 << CPOL) | (1 << CPHA);
	//			break;
	//	}
	//
	//	if ((F_CPU / 2) <= bitrate)
	//	{
	//		configuration->spsr |= (1 << SPI2X);
	//	}
	//	else if ((F_CPU / 4) <= bitrate)
	//	{
	//	}
	//	else if ((F_CPU / 8) <= bitrate)
	//	{
	//		configuration->spcr |= (1 << SPR0);
	//		configuration->spsr |= (1 << SPI2X);
	//	}
	//	else if ((F_CPU / 16) <= bitrate)
	//	{
	//		configuration->spcr |= (1 << SPR0);
	//	}
	//	else if ((F_CPU / 32) <= bitrate)
	//	{
	//		configuration->spcr |= (1 << SPR1);
	//		configuration->spsr |= (1 << SPI2X);
	//	}
	//	else if ((F_CPU / 64) <= bitrate)
	//	{
	//		configuration->spcr |= (1 << SPR1);
	//	}
	//	else if ((F_CPU / 128) <= bitrate)
	//	{
	//		configuration->spcr |= (1 << SPR1) | (1 << SPR0);
	//	}
	//	else
	//	{
	//		// error bitrate too high
	//	}
	//
	//	configuration->csPin = csPin;
	//	configuration->completed = completed;
	//
	//	GPIO_SetupPin(csPin, GPIO_MODE_OUTPUT_HIGH);

	enabled = true;
}

void Timer_Transfer(timer_configuration* configuration, uint8_t* output, uint8_t* input, uint8_t length)
{
	//	if (Queue_IsFull(transferQueue))
	//	{
	//		// error
	//		return;
	//	}

	bool isIdle;
	//	atomic(isIdle = Queue_IsEmpty(transferQueue));
	//
	//	transfer_command* cmd = Queue_Head(transferQueue);
	//
	//	cmd->configuration = configuration;
	//	cmd->output = output;
	//	cmd->input = input;
	//	cmd->length = length;
	//
	//	Queue_AdvanceHead(transferQueue);

	if (isIdle) // just check some register bit instead of isIdle?
	{
		// power up peripheral
		// enable interrupt
		//		StartTransfer();
	}
}

// Internals

//static void StartTransfer()
//{
//	currentTransfer = Queue_Tail(transferQueue);
//
//	GPIO_ClearPin(currentTransfer->configuration->csPin);
//
//	currentOutput = currentTransfer->output;
//	currentInput = currentTransfer->input;
//	remainingBytes = currentTransfer->length;
//
//	SPCR = currentTransfer->configuration->spcr;
//	SPSR = currentTransfer->configuration->spsr;
//
//	SPDR = *currentOutput;
//}

#endif
