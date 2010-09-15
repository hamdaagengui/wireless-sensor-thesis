#ifndef RADIODRIVER_H_
#define RADIODRIVER_H_

#include "../DefaultConfiguration.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/RadioDriver.h"
#else
#error "No radio driver found for the selected processor!"
#endif

#endif /* RADIODRIVER_H_ */
