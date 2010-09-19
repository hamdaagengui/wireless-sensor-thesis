/*
 * HardwareAbstractionLayer.h
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#ifndef HARDWAREABSTRACTIONLAYER_H_
#define HARDWAREABSTRACTIONLAYER_H_

#include "GPIO.h"
#include "IIC.h"
#include "RadioDriver.h"
#include "SPI.h"
#include "PowerManager.h"

extern void HardwareAbstractionLayer_Initialize();
extern void HardwareAbstractionLayer_Start();

#endif /* HARDWAREABSTRACTIONLAYER_H_ */
