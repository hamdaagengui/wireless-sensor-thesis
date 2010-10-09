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
#include "../../Diagnostics/Message.h"

#define SPI_PRESCALER_2																							0x04
#define SPI_PRESCALER_4																							0x00
#define SPI_PRESCALER_8																							0x05
#define SPI_PRESCALER_16																						0x01
#define SPI_PRESCALER_32																						0x06
#define SPI_PRESCALER_64																						0x02
#define SPI_PRESCALER_128																						0x03

static bool enabled = false;

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

void SPI_Subscribe(spi_configuration* configuration)
{
	enabled = true;
}

void SPI_Start()
{
	if (enabled)
	{
		power_spi_enable();


		// basic initialization - only that which is not set by each separate transfers configuration.
		Queue_Initialize(transferQueue, sizeof(transfer_command), SPI_TRANSFER_QUEUE_SIZE);

		PORTB |= (1 << 2) | (1 << 1);

		uint8_t dummy = SPSR; // make sure interrupt flag is cleared
		dummy = SPDR;
	}
	else
	{
		power_spi_disable();
	}
}

void SPI_CreateConfiguration(spi_configuration* configuration, uint32_t bitrate, uint8_t phase, uint8_t polarity, uint8_t dataOrder, uint8_t csPin, completion_handler completed)
{
	// calc prescaler
	// spi2x

	configuration->spcr = (1 << SPIE) | (1 << SPE) | (1 << MSTR);
	configuration->spsr = 0;
}

void SPI_Transfer(spi_configuration* configuration, uint8_t* output, uint8_t* input, uint8_t length)
{
	if (Queue_IsFull(transferQueue))
	{
		// error
		return;
	}

	bool isIdle;
	atomic(isIdle = Queue_IsEmpty(transferQueue));

	transfer_command* cmd = Queue_Head(transferQueue);

	cmd->configuration = configuration;
	cmd->output = output;
	cmd->input = input;
	cmd->length = length;

	Queue_AdvanceHead(transferQueue);

	if (isIdle) // just check some register bit instead of isIdle?
	{
		// power up peripheral
		// enable interrupt
		StartTransfer();
	}

	Message_FrameworkMessage(PSTR(""));
}

// Internals

static void StartTransfer()
{
	currentTransfer = Queue_Tail(transferQueue);

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
		EventDispatcher_Notify(currentTransfer->configuration->completed);
		Queue_AdvanceTail(transferQueue);

		if (Queue_IsEmpty(transferQueue))
		{
			// power down peripheral
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
