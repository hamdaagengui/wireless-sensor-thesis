/*
 * GPIO.h
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <Globals.h>
#include "../DefaultConfiguration.h"

typedef struct
{
	uint8_t mode :2;
	uint8_t outputValue :1;
	uint8_t inputValue :1;
} gpioPin;

typedef struct
{
	uint32_t bitrate;
	uint8_t csPin;
	completionHandler completed;
} gpioConfiguration;

enum
{
	GPIO_INPUT,
	GPIO_INPUT_PULL_UP,
	GPIO_OUTPUT
};

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/GPIO.h"
#else
#error "No GPIO driver found for the selected processor!"
#endif

extern void GPIO_Initialize();
extern void GPIO_Subscribe(gpioConfiguration* configuration);
extern void GPIO_Start();

extern bool GPIO_ReadPin(uint8_t pin);

#endif /* GPIO_H_ */
