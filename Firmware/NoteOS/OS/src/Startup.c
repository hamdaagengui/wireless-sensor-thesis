/*
 * Startup.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "NoteOS.h"
#include <util/delay.h>

// Entry points to user code.
#ifdef CONFIGURATION_H_
extern void Start();
#endif

uint8_t serialNumber = 0;

int main()
{
	PORTF = 0x00;
	DDRF = 0xff;


	//	GPIO_SetupPin(GPIOE2, GPIO_MODE_OUTPUT_HIGH);
	//	GPIO_SetupPin(GPIOE3, GPIO_MODE_OUTPUT_HIGH);
	//	GPIO_SetupPin(GPIOE4, GPIO_MODE_OUTPUT_HIGH);

	PORTD = 0b01010101; // SN configuration
	DDRD = 0b10101010;
	DDRD = 0b10101010;
	DDRD = 0b10101010;
	DDRD = 0b10101010;
	serialNumber += ReadBit(PIND, 0) ? 0 : 1;
	serialNumber += ReadBit(PIND, 2) ? 0 : 2;
	serialNumber += ReadBit(PIND, 4) ? 0 : 4;
	//	sn += ReadBit(PIND, 6) ? 0 : 8;

	Diagnostics_Initialize();


	//	RadioDriver_Initialize();
	//	sei();
	//
	//	if (ReadBit(PIND, 6) == 0)
	//	{
	//		Leds_YellowOn();
	//
	//		uint8_t b[64];
	//		while (1)
	//		{
	//			Leds_RedOn();
	//
	//			for (uint16_t i = 0; i < 100; i++)
	//			{
	//				RadioDriver_Send(b, sizeof(b));
	//				RadioDriver_WaitForSendToComplete();
	//				Leds_GreenToggle();
	//			}
	//
	//			Leds_GreenOff();
	//			Leds_RedOff();
	//
	//			_delay_ms(500);
	//		}
	//	}
	//	else
	//	{
	//		Leds_GreenOn();
	//
	//		RadioDriver_EnableReceiveMode();
	//
	//		int16_t avr = 0;
	//		uint8_t counter = 0;
	//
	//		while (1)
	//		{
	//			int8_t r = RadioDriver_GetRssi();
	//			avr = (avr * 15 + r) / 16;
	//
	//
	//			//			if (++counter >= 50)
	//			//			{
	//			//				counter = 0;
	//			//				Diagnostics_SendRaw(avr);
	//			//			}
	//
	//			bool clear = false;
	//			for (uint8_t i = 0; i < 5; i++)
	//			{
	//				if (r < (avr - 10))
	//				{
	//					clear = true;
	//				}
	//			}
	//
	//			if (clear)
	//			{
	//				Leds_RedOff();
	//			}
	//			else
	//			{
	//				Leds_RedOn();
	//			}
	//
	//			static bool state = false;
	//			if (state != clear)
	//			{
	//				state = clear;
	//				if (clear == false)
	//				{
	//					Diagnostics_SendRaw('@');
	//				}
	//			}
	//
	//			Leds_YellowToggle();
	//
	//			_delay_ms(1);
	//		}
	//	}

	MemoryManager_Initialize();
	EventDispatcher_Initialize();

	HardwareAbstractionLayer_Initialize();

	Network_Initialize();
	Network_Start();


#ifdef BOARDSUPPORTPACKAGE_H_
	BoardSupportPackage_Initialize();
#endif

	SensorManager_InstallSensor(&internalInterface);

	sei();


	// Start user application (only if a configuration file exist (used as an indicator of a real application as opposed to building documentation))
#ifdef CONFIGURATION_H_
	Start();
#endif

#if MASTER_NODE == 0
	Diagnostics_SendEvent(DIAGNOSTICS_SLAVE_NODE_RUNNING);
#else
	Diagnostics_SendEvent(DIAGNOSTICS_MASTER_NODE_RUNNING);
#endif

	while (true)
	{
		// Pump the event sub system
		EventDispatcher_Dispatch();


		// Sleep
		PowerManager_PowerDown();
	}

	return 0;
}
