/*
 * PlatformAbstractionLayer.h
 *
 *  Created on: 21/11/2010
 *      Author: coma
 */

#ifndef PLATFORMABSTRACTIONLAYER_H_
#define PLATFORMABSTRACTIONLAYER_H_

#include "../DefaultConfiguration.h"

#if defined(PLATFORM_STK600_ATMEGA128RFA1)
#include "STK600_ATmega128RFA1/PlatformAbstractionLayer.h"
#else
#define PLATFORM_NONE
#endif

#ifndef PLATFORM_NONE
extern void PlatformAbstractionLayer_Initialize();
extern void PlatformAbstractionLayer_Start();
#endif

#endif /* PLATFORMABSTRACTIONLAYER_H_ */
