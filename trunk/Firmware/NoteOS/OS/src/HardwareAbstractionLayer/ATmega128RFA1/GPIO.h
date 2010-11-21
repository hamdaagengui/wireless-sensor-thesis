/*
 * GPIO.h
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

typedef enum
{
	GPIOB0,
	GPIOB1,
	GPIOB2,
	GPIOB3,
	GPIOB4,
	GPIOB5,
	GPIOB6,
	GPIOB7,

	GPIOD0,
	GPIOD1,
	GPIOD2,
	GPIOD3,
	GPIOD4,
	GPIOD5,
	GPIOD6,
	GPIOD7,

	GPIOE0,
	GPIOE1,
	GPIOE2,
	GPIOE3,
	GPIOE4,
	GPIOE5,
	GPIOE6,
	GPIOE7,

	GPIOF0,
	GPIOF1,
	GPIOF2,
	GPIOF3,
	GPIOF4,
	GPIOF5,
	GPIOF6,
	GPIOF7,

	GPIOG0,
	GPIOG1,
	GPIOG2,
	GPIOG3,
	GPIOG4,
	GPIOG5,
} gpio_pin;

typedef enum
{
	GPIOB = GPIOB0,
	GPIOD = GPIOD0,
	GPIOE = GPIOE0,
	GPIOF = GPIOF0,
	GPIOG = GPIOG0
} gpio_port;
