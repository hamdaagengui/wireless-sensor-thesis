/*
 * PowerManager.h
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#ifndef POWERMANAGER_H_
#define POWERMANAGER_H_

#include "../Globals.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/PowerManager.h"
#else
#define POWERMANAGER_UNIMPLEMENTED
#endif

#ifndef POWERMANAGER_UNIMPLEMENTED

extern void PowerManager_Initialize();
extern void PowerManager_RequestResource(processor_resource resource);
extern void PowerManager_ReleaseResource(processor_resource resource);
extern void PowerManager_PowerDown();

#endif

#endif /* POWERMANAGER_H_ */
