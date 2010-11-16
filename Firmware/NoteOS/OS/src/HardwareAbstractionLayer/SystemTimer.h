/*
 * SystemTimer.h
 *
 *  Created on: 16/11/2010
 *      Author: coma
 */

#ifndef SYSTEMTIMER_H_
#define SYSTEMTIMER_H_

#include "../Globals.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/SystemTimer.h"
#else
#error "No system timer found for the selected processor!"
#endif

extern void SystemTimer_Initialize();
extern void SystemTimer_Enable();
extern void SystemTimer_Disable();
extern void SystemTimer_SetPeriod(uint16_t period);
extern uint16_t SystemTimer_Reset();

#endif /* SYSTEMTIMER_H_ */
