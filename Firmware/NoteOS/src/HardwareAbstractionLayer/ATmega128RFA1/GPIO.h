/*
 * GPIO.h
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#define GPIO_PORT_MASK																							0xf1
#define GPIO_PIN_MASK																								0x07

enum
{
//	GPIOA = 0,
	GPIOB = 8,
//	GPIOC = 16,
	GPIOD = 24,
	GPIOE = 32,
	GPIOF = 40,
	GPIOG = 48
};

enum
{
	GPIOB0 = GPIOB,
	GPIOB1,
	GPIOB2,
	GPIOB3,
	GPIOB4,
	GPIOB5,
	GPIOB6,
	GPIOB7,

	GPIOD0 = GPIOD,
	GPIOD1,
	GPIOD2,
	GPIOD3,
	GPIOD4,
	GPIOD5,
	GPIOD6,
	GPIOD7,

	GPIOE0 = GPIOE,
	GPIOE1,
	GPIOE2,
	GPIOE3,
	GPIOE4,
	GPIOE5,
	GPIOE6,
	GPIOE7,

	GPIOF0 = GPIOF,
	GPIOF1,
	GPIOF2,
	GPIOF3,
	GPIOF4,
	GPIOF5,
	GPIOF6,
	GPIOF7,

	GPIOG0 = GPIOG,
	GPIOG1,
	GPIOG2,
	GPIOG3,
	GPIOG4,
	GPIOG5,
};
