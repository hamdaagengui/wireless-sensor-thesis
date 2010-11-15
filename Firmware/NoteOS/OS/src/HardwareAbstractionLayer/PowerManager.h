/*
 * PowerManager.h
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#ifndef POWERMANAGER_H_
#define POWERMANAGER_H_

#include "../Globals.h"
#include "../EventSubsystem/EventDispatcher.h"
#include "../DefaultConfiguration.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/PowerManager.h"
#else
#error "No power management driver found for the selected processor!"
#endif

extern void PowerManager_Initialize();
extern void PowerManager_PowerDown();

#endif /* POWERMANAGER_H_ */
