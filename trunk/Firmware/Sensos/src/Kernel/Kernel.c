/*
 * Kernel.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include <Globals.h>
#include <DefaultConfiguration.h>
#include <PeripheralSubsystem/Drivers/ADC.h>
#include <PeripheralSubsystem/PeripheralDataDistributor.h>

extern void Start();

int main()
{
	// Initialize sub systems
	//	Kernel_Initialize();
	//	Network_Initialize();
	PeripheralDataDistributor_Initialize();
	ADC_Initialize();


	// Start user application
	Start();


	// Start sub systems
	ADC_Start();


	// Run system
	sei();
	while (true)
	{
		// Pump PDD sub system
		PeripheralDataDistributor_ProcessEvents();


		// Pump SDD sub system

		// Sleep

	}

	return 0;
}
