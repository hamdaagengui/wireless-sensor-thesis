/*
 * MAX6662.h
 *
 *  Created on: 09/10/2010
 *      Author: coma
 */

#ifndef MAX6662_H_
#define MAX6662_H_

#include "../Globals.h"
#include "../HardwareAbstractionLayer/HardwareAbstractionLayer.h"

typedef struct
{
	spi_configuration spiConfiguration;
} max6662_configuration;

void MAX6662_CreateConfiguration(max6662_configuration* configuration, gpio_pin csPin);
void MAX6662_Initialize(max6662_configuration* configuration);
void MAX6662_GetTemperature(max6662_configuration* configuration, int16_t* temperature, completion_handler handler);

#endif /* MAX6662_H_ */
