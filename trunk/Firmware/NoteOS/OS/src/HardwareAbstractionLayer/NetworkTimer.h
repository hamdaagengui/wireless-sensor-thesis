/*
 * NetworkTimer.h
 *
 *  Created on: 11/10/2010
 *      Author: coma
 */

#ifndef NETWORKTIMER_H_
#define NETWORKTIMER_H_

#include "../Globals.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/NetworkTimer.h"
#else
#define NETWORKTIMER_UNIMPLEMENTED
#endif

#ifndef NETWORKTIMER_UNIMPLEMENTED

extern void NetworkTimer_Initialize();
extern void NetworkTimer_SetTimerPeriod(uint16_t value);
extern void NetworkTimer_SetTimerValue(uint16_t value);

#endif

#endif /* NETWORKTIMER_H_ */
