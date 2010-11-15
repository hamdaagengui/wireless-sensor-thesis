/*
 * Leds.h
 *
 *  Created on: 25/10/2010
 *      Author: coma
 */

#ifndef LEDS_H_
#define LEDS_H_

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/Leds.h"
#else
#error "No LED driver found for the selected processor!"
#endif

extern void Leds_Initialize();
extern void Leds_GreenOn();
extern void Leds_GreenOff();
extern void Leds_GreenToggle();
extern void Leds_YellowOn();
extern void Leds_YellowOff();
extern void Leds_YellowToggle();
extern void Leds_RedOn();
extern void Leds_RedOff();
extern void Leds_RedToggle();

#endif /* LEDS_H_ */
