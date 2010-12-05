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

uint8_t serialNumber[16];

int main()
{
	PORTF = 0x00;
	DDRF = 0xff;

	PORTD = 0b01010101; // SN configuration
	DDRD = 0b10101010;
	DDRD = 0b10101010;
	DDRD = 0b10101010;
	DDRD = 0b10101010;
	serialNumber[0] += ReadBit(PIND, 0) ? 0 : 1;
	serialNumber[0] += ReadBit(PIND, 2) ? 0 : 2;
	serialNumber[0] += ReadBit(PIND, 4) ? 0 : 4;
	//	sn += ReadBit(PIND, 6) ? 0 : 8;

	Diagnostics_Initialize();

	MemoryManager_Initialize();

	EventDispatcher_Initialize();

	HardwareAbstractionLayer_Initialize();

	Network_Initialize();
	Network_Start();

	_delay_ms(serialNumber[0] * 50);

	sei();

	SensorManager_InstallSensor(&internalInterface);


#ifdef BOARDSUPPORTPACKAGE_H_
	BoardSupportPackage_Initialize();
#endif


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
