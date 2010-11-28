/*
 * PlatformAbstractionLayer.c
 *
 *  Created on: 21/11/2010
 *      Author: coma
 */

#include "../PlatformAbstractionLayer.h"

max6662_configuration max6662Configuration;

void PlatformAbstractionLayer_Initialize()
{
	Leds_Initialize();

	MAX6662_CreateConfiguration(&max6662Configuration, GPIOB4);
	MAX6662_Initialize(&max6662Configuration);
}

void PlatformAbstractionLayer_Start()
{

}
