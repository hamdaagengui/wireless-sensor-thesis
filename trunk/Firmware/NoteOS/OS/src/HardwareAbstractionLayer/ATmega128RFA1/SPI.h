/*
 * SPI.h
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

typedef struct
{
	uint8_t spcr;
	uint8_t spsr;
	gpio_pin csPin;
} spi_configuration;
