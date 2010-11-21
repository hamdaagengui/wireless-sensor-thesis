/*
 * GPIO.h
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "../Globals.h"
#include "../DefaultConfiguration.h"

typedef enum
{
	GPIO_MODE_INPUT_FLOATING,
	GPIO_MODE_INPUT_PULL_UP,
	GPIO_MODE_OUTPUT_LOW,
	GPIO_MODE_OUTPUT_HIGH
} gpio_mode;

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/GPIO.h"
#else
#error "No GPIO driver found for the selected processor!"
#endif

extern void GPIO_SetupPin(gpio_pin pin, uint8_t mode);
extern bool GPIO_ReadPin(gpio_pin pin);
extern void GPIO_WritePin(gpio_pin pin, bool value);
extern void GPIO_ClearPin(gpio_pin pin);
extern void GPIO_SetPin(gpio_pin pin);
extern void GPIO_TogglePin(gpio_pin pin);

extern void GPIO_SetupPort(gpio_port port, uint8_t mode);
extern uint8_t GPIO_ReadPort(gpio_port port);
extern void GPIO_WritePort(gpio_port port, uint8_t value);

#endif /* GPIO_H_ */
