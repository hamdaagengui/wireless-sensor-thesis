/*
 * NonVolatileStorage.h
 *
 *  Created on: 12/10/2010
 *      Author: Coma
 */

#ifndef NONVOLATILESTORAGE_H_
#define NONVOLATILESTORAGE_H_

#include "../Globals.h"
#include "../EventSubsystem/EventDispatcher.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/NonVolatileStorage.h"
#else
#define NONVOLATILESTORAGE_UNIMPLEMENTED
#endif

#ifndef NONVOLATILESTORAGE_UNIMPLEMENTED

extern void NonVolatileStorage_Read(const void* object, void* buffer, uint8_t length);
extern void NonVolatileStorage_Write(void* object, const void* buffer, uint8_t length);

#endif

#endif /* NONVOLATILESTORAGE_H_ */
