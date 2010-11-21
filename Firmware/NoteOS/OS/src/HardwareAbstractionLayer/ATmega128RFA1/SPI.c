/*
 * SPI.c
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include <avr/power.h>
#include "../SPI.h"
#include "../../Collections/Queue.h"
#include "../../EventSubsystem/EventDispatcher.h"
#include "../GPIO.h"

static bool enabled = false;

typedef struct
{
	spi_configuration* configuration;
	uint8_t* output;
	uint8_t* input;
	uint8_t length;
} operation;
static uint8_t operationQueue[Queue_CalculateSize(sizeof(operation), SPI_OPERATION_QUEUE_SIZE)];

static operation* currentOperation;
static uint8_t* currentOutput;
static uint8_t* currentInput;
static uint8_t remainingBytes;

static void ExecuteOperation();

void SPI_Initialize()
{
	Queue_Initialize(operationQueue, sizeof(operation), SPI_OPERATION_QUEUE_SIZE);
}

void SPI_Start()
{
	if (enabled)
	{
		//power_spi_enable();


		PORTB |= (1 << 2) | (1 << 1);

		uint8_t dummy = SPSR; // make sure interrupt flag is cleared
		dummy = SPDR;
	}
	else
	{
		//power_spi_disable();
	}
}

void SPI_CreateConfiguration(spi_configuration* configuration, uint32_t bitrate, spi_data_mode mode, spi_data_order order, gpio_pin csPin, completion_handler completed)
{
	configuration->spcr = (1 << SPIE) | (1 << SPE) | (1 << MSTR);
	configuration->spsr = 0;

	switch (order)
	{
		case SPI_DATA_ORDER_LSB_FIRST:
			configuration->spcr |= (1 << DORD);
			break;

		case SPI_DATA_ORDER_MSB_FIRST:
			break;
	}

	switch (mode)
	{
		case SPI_DATA_MODE_LEADING_RISING:
			break;

		case SPI_DATA_MODE_LEADING_FALLING:
			configuration->spcr |= (1 << CPOL);
			break;

		case SPI_DATA_MODE_TRAILING_FALLING:
			configuration->spcr |= (1 << CPHA);
			break;

		case SPI_DATA_MODE_TRAILING_RISING:
			configuration->spcr |= (1 << CPOL) | (1 << CPHA);
			break;
	}

	if ((F_CPU / 2) <= bitrate)
	{
		configuration->spsr |= (1 << SPI2X);
	}
	else if ((F_CPU / 4) <= bitrate)
	{
		// use default
	}
	else if ((F_CPU / 8) <= bitrate)
	{
		configuration->spcr |= (1 << SPR0);
		configuration->spsr |= (1 << SPI2X);
	}
	else if ((F_CPU / 16) <= bitrate)
	{
		configuration->spcr |= (1 << SPR0);
	}
	else if ((F_CPU / 32) <= bitrate)
	{
		configuration->spcr |= (1 << SPR1);
		configuration->spsr |= (1 << SPI2X);
	}
	else if ((F_CPU / 64) <= bitrate)
	{
		configuration->spcr |= (1 << SPR1);
	}
	else if ((F_CPU / 128) <= bitrate)
	{
		configuration->spcr |= (1 << SPR1) | (1 << SPR0);
	}
	else
	{
		// error bitrate too high
	}

	configuration->csPin = csPin;
	configuration->completed = completed;

	GPIO_SetupPin(csPin, GPIO_MODE_OUTPUT_HIGH);

	enabled = true;
}

void SPI_Transfer(spi_configuration* configuration, uint8_t* output, uint8_t* input, uint8_t length)
{
	if (Queue_IsFull(operationQueue))
	{
		return;
	}

	bool isIdle;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		isIdle = Queue_IsEmpty(operationQueue);
	}

	operation* o = Queue_Head(operationQueue);

	o->configuration = configuration;
	o->output = output;
	o->input = input;
	o->length = length;

	Queue_AdvanceHead(operationQueue);

	if (isIdle) // just check some register bit instead of isIdle?
	{
		// power up peripheral
		ExecuteOperation();
	}
}

// Internals

static void ExecuteOperation()
{
	// power up if powered off

	currentOperation = Queue_Tail(operationQueue);

	GPIO_ClearPin(currentOperation->configuration->csPin);

	currentOutput = currentOperation->output;
	currentInput = currentOperation->input;
	remainingBytes = currentOperation->length;

	SPCR = currentOperation->configuration->spcr;
	SPSR = currentOperation->configuration->spsr;

	SPDR = *currentOutput;
}

ISR(SPI_STC_vect)
{
	*currentInput = SPDR;

	if (--remainingBytes == 0)
	{
		GPIO_SetPin(currentOperation->configuration->csPin);

		EventDispatcher_Notify(currentOperation->configuration->completed);

		Queue_AdvanceTail(operationQueue);
		if (Queue_IsEmpty(operationQueue))
		{
			// power down peripheral
			SPCR = 0x00; // disable SPI
		}
		else
		{
			ExecuteOperation();
		}
	}
	else
	{
		currentInput++;
		currentOutput++;
		SPDR = *currentOutput;
	}
}

#endif
