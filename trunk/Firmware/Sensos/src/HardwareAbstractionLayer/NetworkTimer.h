/*
 * NetworkTimer.h
 *
 *  Created on: 11/10/2010
 *      Author: coma
 */

#ifndef NETWORKTIMER_H_
#define NETWORKTIMER_H_

#include "../Globals.h"
#include "../DefaultConfiguration.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/NetworkTimer.h"
#else
#error "No network timer found for the selected processor!"
#endif

extern void NetworkTimer_Initialize(notification_handler handler);
extern void NetworkTimer_SetTimerValue(uint16_t value);
extern void NetworkTimer_SetTimerPeriod(uint16_t value);

#endif /* NETWORKTIMER_H_ */
