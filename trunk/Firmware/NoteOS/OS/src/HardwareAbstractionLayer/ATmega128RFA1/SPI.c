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

static bool enabled;

typedef struct
{
	spi_configuration* configuration;
	uint8_t* output;
	uint8_t* input;
	uint8_t length;
} transfer_command;
static uint8_t transferQueue[Queue_CalculateSize(sizeof(transfer_command), SPI_TRANSFER_QUEUE_SIZE)];

static transfer_command* currentTransfer;
static uint8_t* currentOutput;
static uint8_t* currentInput;
static uint8_t remainingBytes;

static void StartTransfer();

void SPI_Initialize()
{
}

void SPI_Start()
{
	if (enabled)
	{
		//power_spi_enable();


		// basic initialization - only that which is not set by each separate transfers configuration.
		Queue_Initialize(transferQueue, sizeof(transfer_command), SPI_TRANSFER_QUEUE_SIZE);

		PORTB |= (1 << 2) | (1 << 1);

		uint8_t dummy = SPSR; // make sure interrupt flag is cleared
		dummy = SPDR;
	}
	else
	{
		//power_spi_disable();
	}
}

void SPI_CreateConfiguration(spi_configuration* configuration, uint32_t bitrate, spi_data_mode mode, spi_data_order order, uint8_t csPin, completion_handler completed)
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
	if (Queue_IsFull(transferQueue))
	{
		// error
		return;
	}

	bool isIdle;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		isIdle = Queue_IsEmpty(transferQueue);
	}

	transfer_command* cmd = Queue_Head(transferQueue);

	cmd->configuration = configuration;
	cmd->output = output;
	cmd->input = input;
	cmd->length = length;

	Queue_AdvanceHead(transferQueue);

	if (isIdle) // just check some register bit instead of isIdle?
	{
		// power up peripheral
		StartTransfer();
	}
}

// Internals

static void StartTransfer()
{
	currentTransfer = Queue_Tail(transferQueue);

	GPIO_ClearPin(currentTransfer->configuration->csPin);

	currentOutput = currentTransfer->output;
	currentInput = currentTransfer->input;
	remainingBytes = currentTransfer->length;

	SPCR = currentTransfer->configuration->spcr;
	SPSR = currentTransfer->configuration->spsr;

	SPDR = *currentOutput;
}

ISR(SPI_STC_vect)
{
	*currentInput = SPDR;

	if (--remainingBytes == 0)
	{
		GPIO_SetPin(currentTransfer->configuration->csPin);

		EventDispatcher_Notify(currentTransfer->configuration->completed);

		Queue_AdvanceTail(transferQueue);
		if (Queue_IsEmpty(transferQueue))
		{
			// power down peripheral
			SPCR = 0x00; // disable SPI
		}
		else
		{
			StartTransfer();
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
