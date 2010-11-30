/*
 * Startup.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "NoteOS.h"

// Entry points to user code.
#ifdef CONFIGURATION_H_
extern void Start();
#endif

uint8_t serialNumber = 0;

int main()
{
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

	Diagnostics_SendEvent(DIAGNOSTICS_SYSTEM_INITIALIZED);

	while (true)
	{
		// Pump the event sub system
		EventDispatcher_Dispatch();


		// Sleep
		PowerManager_PowerDown();
	}

	return 0;
}
