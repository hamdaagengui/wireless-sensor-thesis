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
#include "../NetworkSubsystem/Network.h"

// Entry point to user code.
extern void Start();

uint32_t sn = 0;

int main()
{
	PORTF = 0x00;
	DDRF = 0xff;


	// Initialize IO ports
	DDRE = 0x1c; // LEDs, button and UART
	PORTE = 0x3c;

	PORTD = 0b01010101; // Id configuration
	DDRD = 0b10101010;
	sn += ReadBit(PIND, 0) ? 0 : 1;
	sn += ReadBit(PIND, 2) ? 0 : 2;
	sn += ReadBit(PIND, 4) ? 0 : 4;
	sn += ReadBit(PIND, 6) ? 0 : 8;


	// Initialize sub systems
	Network_Initialize();
	//	PowerManager_Initialize();
	EventDispatcher_Initialize();
	// Initialize peripherals
	//	HardwareAbstractionLayer_Initialize();


	// Start user application
	Start();


	// Start sub systems
	EventDispatcher_Start();
	// Start peripherals
	//	HardwareAbstractionLayer_Start();

	bool connect = false;


	// Run system
	sei();
	while (true)
	{
		// Pump the event sub system
		EventDispatcher_Dispatch();


		// Sleep
		PowerManager_PowerDown();


#ifdef NETWORK_MASTER_NODE
		if (((PIND & (1 << 5)) == 0) && (connect == false))
		{
			connect = true;
			Network_ConfigureNode(1, 0, 20);
		}
#endif
	}

	return 0;
}
