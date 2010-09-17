/*
 * SPI.c
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../SPI.h"
#include "../../Utils/Queue.h"

typedef struct
{
	spiConfiguration* configuration;
	uint8_t* output;
	uint8_t* input;
	uint8_t length;
} transferCommand;
static uint8_t transferQueue[sizeof(queue) + SPI_TRANSFER_QUEUE_SIZE * sizeof(transferCommand)];

void SPI_Initialize()
{
	Queue_Initialize(transferQueue, sizeof(transferCommand), SPI_TRANSFER_QUEUE_SIZE);
}

void SPI_Subscribe(spiConfiguration* configuration)
{

}

void SPI_Start()
{

}

void SPI_Transfer(spiConfiguration* configuration, uint8_t* output, uint8_t* input, uint8_t length)
{
	((transferCommand*) Queue_Head(transferQueue))->configuration = configuration;
	((transferCommand*) Queue_Head(transferQueue))->output = output;
	((transferCommand*) Queue_Head(transferQueue))->input = input;
	((transferCommand*) Queue_Head(transferQueue))->length = length;
}

#endif
