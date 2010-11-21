/*
 * HardwareAbstractionLayer.c
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include "../HardwareAbstractionLayer.h"

void HardwareAbstractionLayer_Initialize()
{
	// PRRx to stop clocks to all peripherals (enabled in their respective Initialize or Start functions)
	// ADC page 410
	// AC p 407
	// Brown out p 178
	// Vref p 179
	// Watchdog p 180
	// Disable digital input buffers for analog input pins p 409 p 433
	// Disable OCDEN and JTAGEN fuses. Set JTD in MCUCR
	// Symbol counter p 133
	// SRAM DR p 163
	// PM p 503


	ADC_Initialize();
	IIC_Initialize();
	NetworkTimer_Initialize();
	// PowerManager_Initialize();
	SPI_Initialize();
	StopWatch_Initialize();
	SystemTimer_Initialize();
}

void HardwareAbstractionLayer_Start()
{
	ADC_Start();
	IIC_Start();
	// PowerManager_Start();
	SPI_Start();
}

#endif
