/*
 * Startup.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "../Globals.h"
#include "../DefaultConfiguration.h"
#include "../HardwareAbstractionLayer/HardwareAbstractionLayer.h"
#include "../PlatformAbstractionLayer/PlatformAbstractionLayer.h"
#include "../MemorySubsystem/MemoryManager.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../NetworkSubsystem/Network.h"
#include "../Diagnostics/Diagnostics.h"

#include <util/delay.h>

// Entry points to user code.
#ifdef CONFIGURATION_H_
extern void Start();
#endif

uint32_t sn = 0;

int main()
{


	//
	//
	//

	PORTD = 0b01010101; // Id configuration
	DDRD = 0b10101010;
	sn += ReadBit(PIND, 0) ? 0 : 1;
	sn += ReadBit(PIND, 2) ? 0 : 2;
	sn += ReadBit(PIND, 4) ? 0 : 4;
	//	sn += ReadBit(PIND, 6) ? 0 : 8;

	//
	//
	//


	Diagnostics_Initialize(sn);


	// Initialize hardware
	HardwareAbstractionLayer_Initialize();


	// Initialize sub systems
	MemoryManager_Initialize();
	Network_Initialize();
	Network_SetAddress(sn);
	//	PowerManager_Initialize();
	EventDispatcher_Initialize();


	// Initialize peripherals
	HardwareAbstractionLayer_Initialize();


#ifdef PLATFORMABSTRACTIONLAYER_H_
	PlatformAbstractionLayer_Initialize();
#endif


	// Start sub systems

	// Start hardware
	HardwareAbstractionLayer_Start();


#ifdef PLATFORMABSTRACTIONLAYER_H_
	PlatformAbstractionLayer_Start();
#endif


	// Start user application
#ifdef CONFIGURATION_H_
	Start();
#endif


	// Run system
	sei();

	Diagnostics_SendEvent(DIAGNOSTICS_SYSTEM_INITIALIZED);

	while (true)
	{
		// Pump the event sub system
		EventDispatcher_Dispatch();


		// Sleep
		//	PowerManager_PowerDown();
	}

	return 0;
}
