/*
 * GPIO.h
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "../Globals.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../DefaultConfiguration.h"

typedef struct
{
	uint8_t mode :2;
	uint8_t outputValue :1;
	uint8_t inputValue :1;
	uint8_t :4;
} gpio_pin;

typedef struct
{
	uint32_t bitrate;
	uint8_t csPin;
	completion_handler completed;
} gpio_configuration;

typedef enum
{
	GPIO_MODE_INPUT,
	GPIO_MODE_INPUT_PULL_UP,
	GPIO_MODE_OUTPUT_LOW,
	GPIO_MODE_OUTPUT_HIGH
} gpio_mode;

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/GPIO.h"
#else
#error "No GPIO driver found for the selected processor!"
#endif

extern void GPIO_Initialize();
extern void GPIO_Subscribe(gpio_configuration* configuration);
extern void GPIO_Start();

extern void GPIO_SetupPin(uint8_t pin, uint8_t mode);

extern bool GPIO_ReadPin(uint8_t pin);

extern void GPIO_WritePin(uint8_t pin, uint8_t value);
extern void GPIO_ClearPin(uint8_t pin);
extern void GPIO_SetPin(uint8_t pin);

#endif /* GPIO_H_ */
