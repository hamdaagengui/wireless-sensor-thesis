/*
 * GPIO.c
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../GPIO.h"
#include "../../Globals.h"

void GPIO_SetupPin(uint8_t pin, uint8_t mode)
{
	switch (pin & GPIO_PORT_MASK)
	{
		case GPIOB:
			switch (mode)
			{
				case GPIO_MODE_INPUT:
					ClearBit(DDRB, pin & GPIO_PIN_MASK);
					ClearBit(PORTB, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_INPUT_PULL_UP:
					ClearBit(DDRB, pin & GPIO_PIN_MASK);
					SetBit(PORTB, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_LOW:
					SetBit(DDRB, pin & GPIO_PIN_MASK);
					ClearBit(PORTB, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_HIGH:
					SetBit(DDRB, pin & GPIO_PIN_MASK);
					SetBit(PORTB, pin & GPIO_PIN_MASK);
					break;
			}
			break;

		case GPIOD:
			switch (mode)
			{
				case GPIO_MODE_INPUT:
					ClearBit(DDRD, pin & GPIO_PIN_MASK);
					ClearBit(PORTD, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_INPUT_PULL_UP:
					ClearBit(DDRD, pin & GPIO_PIN_MASK);
					SetBit(PORTD, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_LOW:
					SetBit(DDRD, pin & GPIO_PIN_MASK);
					ClearBit(PORTD, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_HIGH:
					SetBit(DDRD, pin & GPIO_PIN_MASK);
					SetBit(PORTD, pin & GPIO_PIN_MASK);
					break;
			}
			break;

		case GPIOE:
			switch (mode)
			{
				case GPIO_MODE_INPUT:
					ClearBit(DDRE, pin & GPIO_PIN_MASK);
					ClearBit(PORTE, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_INPUT_PULL_UP:
					ClearBit(DDRE, pin & GPIO_PIN_MASK);
					SetBit(PORTE, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_LOW:
					SetBit(DDRE, pin & GPIO_PIN_MASK);
					ClearBit(PORTE, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_HIGH:
					SetBit(DDRE, pin & GPIO_PIN_MASK);
					SetBit(PORTE, pin & GPIO_PIN_MASK);
					break;
			}
			break;

		case GPIOF:
			switch (mode)
			{
				case GPIO_MODE_INPUT:
					ClearBit(DDRF, pin & GPIO_PIN_MASK);
					ClearBit(PORTF, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_INPUT_PULL_UP:
					ClearBit(DDRF, pin & GPIO_PIN_MASK);
					SetBit(PORTF, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_LOW:
					SetBit(DDRF, pin & GPIO_PIN_MASK);
					ClearBit(PORTF, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_HIGH:
					SetBit(DDRF, pin & GPIO_PIN_MASK);
					SetBit(PORTF, pin & GPIO_PIN_MASK);
					break;
			}
			break;

		case GPIOG:
			switch (mode)
			{
				case GPIO_MODE_INPUT:
					ClearBit(DDRG, pin & GPIO_PIN_MASK);
					ClearBit(PORTG, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_INPUT_PULL_UP:
					ClearBit(DDRG, pin & GPIO_PIN_MASK);
					SetBit(PORTG, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_LOW:
					SetBit(DDRG, pin & GPIO_PIN_MASK);
					ClearBit(PORTG, pin & GPIO_PIN_MASK);
					break;
				case GPIO_MODE_OUTPUT_HIGH:
					SetBit(DDRG, pin & GPIO_PIN_MASK);
					SetBit(PORTG, pin & GPIO_PIN_MASK);
					break;
			}
			break;
	}
}

bool GPIO_ReadPin(uint8_t pin)
{
	switch (pin & GPIO_PORT_MASK)
	{
		case GPIOB:
			return ReadBit(PINB, pin & GPIO_PIN_MASK);
			break;

		case GPIOD:
			return ReadBit(PIND, pin & GPIO_PIN_MASK);
			break;

		case GPIOE:
			return ReadBit(PINE, pin & GPIO_PIN_MASK);
			break;

		case GPIOF:
			return ReadBit(PINF, pin & GPIO_PIN_MASK);
			break;

		case GPIOG:
			return ReadBit(PING, pin & GPIO_PIN_MASK);
			break;

		default:
			// error
			return false;
			break;
	}
}

void GPIO_WritePin(uint8_t pin, uint8_t value)
{
	switch (pin & GPIO_PORT_MASK)
	{
		case GPIOB:
			if (value)
			{
				SetBit(PORTB, pin & GPIO_PIN_MASK);
			}
			else
			{
				ClearBit(PORTB, pin & GPIO_PIN_MASK);
			}
			break;

		case GPIOD:
			if (value)
			{
				SetBit(PORTD, pin & GPIO_PIN_MASK);
			}
			else
			{
				ClearBit(PORTD, pin & GPIO_PIN_MASK);
			}
			break;

		case GPIOE:
			if (value)
			{
				SetBit(PORTE, pin & GPIO_PIN_MASK);
			}
			else
			{
				ClearBit(PORTE, pin & GPIO_PIN_MASK);
			}
			break;

		case GPIOF:
			if (value)
			{
				SetBit(PORTF, pin & GPIO_PIN_MASK);
			}
			else
			{
				ClearBit(PORTF, pin & GPIO_PIN_MASK);
			}
			break;

		case GPIOG:
			if (value)
			{
				SetBit(PORTG, pin & GPIO_PIN_MASK);
			}
			else
			{
				ClearBit(PORTG, pin & GPIO_PIN_MASK);
			}
			break;
	}
}

void GPIO_ClearPin(uint8_t pin)
{
	switch (pin & GPIO_PORT_MASK)
	{
		case GPIOB:
			ClearBit(PORTB, pin & GPIO_PIN_MASK);
			break;

		case GPIOD:
			ClearBit(PORTD, pin & GPIO_PIN_MASK);
			break;

		case GPIOE:
			ClearBit(PORTE, pin & GPIO_PIN_MASK);
			break;

		case GPIOF:
			ClearBit(PORTF, pin & GPIO_PIN_MASK);
			break;

		case GPIOG:
			ClearBit(PORTG, pin & GPIO_PIN_MASK);
			break;
	}
}

void GPIO_SetPin(uint8_t pin)
{
	switch (pin & GPIO_PORT_MASK)
	{
		case GPIOB:
			SetBit(PORTB, pin & GPIO_PIN_MASK);
			break;

		case GPIOD:
			SetBit(PORTD, pin & GPIO_PIN_MASK);
			break;

		case GPIOE:
			SetBit(PORTE, pin & GPIO_PIN_MASK);
			break;

		case GPIOF:
			SetBit(PORTF, pin & GPIO_PIN_MASK);
			break;

		case GPIOG:
			SetBit(PORTG, pin & GPIO_PIN_MASK);
			break;
	}
}

#endif
