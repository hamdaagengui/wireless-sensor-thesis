/*
 * SPI.h
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#ifndef SPI_H_
#define SPI_H_

#include <Globals.h>
#include "../EventSubsystem/EventDispatcher.h"
#include "../DefaultConfiguration.h"

typedef struct
{
	uint32_t bitrate;
	uint8_t csPin;
	completionHandler completed;
} spiConfiguration;

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/SPI.h"
#else
#error "No SPI driver found for the selected processor!"
#endif

extern void SPI_Initialize();
extern void SPI_Subscribe(spiConfiguration* configuration);
extern void SPI_Start();

/**
 * Queues a transfer command. OBS Only a pointer to the output buffer is queued so it is not safe to modify the buffer until the completion handler is called!
 */
extern void SPI_Transfer(spiConfiguration* configuration, uint8_t* output, uint8_t* input, uint8_t length);

#endif /* SPI_H_ */
