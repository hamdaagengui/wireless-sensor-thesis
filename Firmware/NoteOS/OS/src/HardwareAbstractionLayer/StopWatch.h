/*
 * StopWatch.h
 *
 *  Created on: 17/11/2010
 *      Author: coma
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include "../Globals.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/StopWatch.h"
#else
#define STOPWATCH_UNIMPLEMENTED
#endif

#ifndef STOPWATCH_UNIMPLEMENTED

extern void StopWatch_Initialize();
extern void StopWatch_Start();
extern uint32_t StopWatch_Stop();

#endif

#endif /* STOPWATCH_H_ */
