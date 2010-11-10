/*
 * SPI.h
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#ifndef SPI_H_
#define SPI_H_

#include "../Globals.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../DefaultConfiguration.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/SPI.h"
#else
#error "No SPI driver found for the selected processor!"
#endif

typedef enum
{
	SPI_DATA_MODE_LEADING_RISING,
	SPI_DATA_MODE_LEADING_FALLING,
	SPI_DATA_MODE_TRAILING_FALLING,
	SPI_DATA_MODE_TRAILING_RISING
} spi_data_mode;

typedef enum
{
	SPI_DATA_ORDER_LSB_FIRST,
	SPI_DATA_ORDER_MSB_FIRST
} spi_data_order;

extern void SPI_Initialize();
extern void SPI_Start();

extern void SPI_CreateConfiguration(spi_configuration* configuration, uint32_t bitrate, spi_data_mode mode, spi_data_order order, uint8_t csPin, completion_handler completed);

/**
 * Queues a transfer command. OBS Only a pointer to the output buffer is queued so it is not safe to modify the buffer until the completion handler is called!
 */
extern void SPI_Transfer(spi_configuration* configuration, uint8_t* output, uint8_t* input, uint8_t length);

#endif /* SPI_H_ */
