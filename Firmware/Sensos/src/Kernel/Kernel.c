/*
 * Kernel.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "../Globals.h"
#include "../DefaultConfiguration.h"
#include "../HardwareAbstractionLayer/HardwareAbstractionLayer.h"
#include "../EventSubsystem/EventDispatcher.h"

// Entry point to user code.
extern void Start();

int main()
{
	// Initialize sub systems
	//	Kernel_Initialize();
	//	Network_Initialize();
	EventDispatcher_Initialize();
	// Initialize peripherals
	HardwareAbstractionLayer_Initialize();


	// Start user application
	Start();


	// Start sub systems
	EventDispatcher_Start();
	// Start peripherals
	HardwareAbstractionLayer_Start();


	// Run system
	sei();
	while (true)
	{
		// Pump the event sub system
		EventDispatcher_Dispatch();


		// Sleep

	}

	return 0;
}
