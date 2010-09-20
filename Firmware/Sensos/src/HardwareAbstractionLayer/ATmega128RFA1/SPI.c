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
#include "../../Diagnostics/Log.h"

static bool enabled = false;

typedef struct
{
	spiConfiguration* configuration;
	uint8_t* output;
	uint8_t* input;
	uint8_t length;
} transferCommand;
static uint8_t transferQueue[Queue_CalculateSize(sizeof(transferCommand), SPI_TRANSFER_QUEUE_SIZE)];

static transferCommand* currentTransfer;
static uint8_t* currentOutput;
static uint8_t* currentInput;
static uint8_t remainingBytes;

static void StartTransfer();

void SPI_Initialize()
{
	Queue_Initialize(transferQueue, sizeof(transferCommand), SPI_TRANSFER_QUEUE_SIZE);
}

void SPI_Subscribe(spiConfiguration* configuration)
{
	enabled = true;
}

void SPI_Start()
{
	if (enabled)
	{
		power_spi_enable();
		// basic initialization - only that which is not set by each separate transfers configuration.
	}
	else
	{
		power_spi_disable();
	}
}

void SPI_Transfer(spiConfiguration* configuration, uint8_t* output, uint8_t* input, uint8_t length)
{
	if (Queue_IsFull(transferQueue))
	{
		// error
		return;
	}

	bool isIdle;
	atomic(isIdle = Queue_IsEmpty(transferQueue));

	transferCommand* cmd = Queue_Head(transferQueue);

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

	Log_FrameworkMessage(PSTR(""));
}

// Internals

static void StartTransfer()
{
	currentTransfer = Queue_Tail(transferQueue);
	currentOutput = currentTransfer->output;
	currentInput = currentTransfer->input;
	remainingBytes = currentTransfer->length;


	// load configuration

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
