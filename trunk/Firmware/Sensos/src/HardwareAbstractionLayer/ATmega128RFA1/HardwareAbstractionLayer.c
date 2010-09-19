/*
 * HardwareAbstractionLayer.c
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#include "../HardwareAbstractionLayer.h"

void HardwareAbstractionLayer_Initialize()
{
	SPI_Initialize();
}

void HardwareAbstractionLayer_Start()
{
	SPI_Start();
}
