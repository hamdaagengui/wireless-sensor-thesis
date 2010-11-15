/*
 * Kernel.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "../Globals.h"
#include "../DefaultConfiguration.h"
#include "../HardwareAbstractionLayer/HardwareAbstractionLayer.h"
#include "../MemorySubsystem/MemoryManager.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../NetworkSubsystem/Network.h"
#include "../Diagnostics/Diagnostics.h"

#include <util/delay.h>

// Entry points to user code.
extern void Initialize();
extern void Start();

uint32_t sn = 0;

int main()
{
	Initialize(); // => Platform_Initialize() ?

	PORTF = 0x00;
	DDRF = 0xff;

	PORTB = 0x00;
	DDRB = 0xff;


	// Initialize IO ports
	DDRE = 0x1c; // LEDs, button and UART
	PORTE = 0x3c;

	PORTD = 0b01010101; // Id configuration
	DDRD = 0b10101010;
	sn += ReadBit(PIND, 0) ? 0 : 1;
	sn += ReadBit(PIND, 2) ? 0 : 2;
	sn += ReadBit(PIND, 4) ? 0 : 4;
	//	sn += ReadBit(PIND, 6) ? 0 : 8;

	Diagnostics_Initialize(sn);


	// Initialize sub systems
	MemoryManager_Initialize();
	Network_Initialize();
	Network_SetAddress(sn);
	//	PowerManager_Initialize();
	EventDispatcher_Initialize();


	// Initialize peripherals
	HardwareAbstractionLayer_Initialize();


	// Start user application
	Start();


	// Start sub systems

	// Start peripherals
	//	HardwareAbstractionLayer_Start();

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
