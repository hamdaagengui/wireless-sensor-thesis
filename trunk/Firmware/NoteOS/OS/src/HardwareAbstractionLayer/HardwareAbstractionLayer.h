/*
 * HardwareAbstractionLayer.h
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#ifndef HARDWAREABSTRACTIONLAYER_H_
#define HARDWAREABSTRACTIONLAYER_H_

#include "ADC.h"
#include "DiagnosticsLink.h"
#include "GPIO.h"
#include "IIC.h"
#include "NetworkTimer.h"
#include "NonVolatileStorage.h"
//#include "PowerManager.h"
#include "RadioDriver.h"
#include "SPI.h"
#include "StopWatch.h"
#include "SystemTimer.h"

extern void HardwareAbstractionLayer_Initialize();
extern void HardwareAbstractionLayer_Start();

#endif /* HARDWAREABSTRACTIONLAYER_H_ */
