/*
 * BoardSupportPackage.h
 *
 *  Created on: 21/11/2010
 *      Author: coma
 */

#ifndef BOARDSUPPORTPACKAGE_H_
#define BOARDSUPPORTPACKAGE_H_

#include "../Globals.h"

#if defined(BOARDSUPPORTPACKAGE_STK600_ATMEGA128RFA1)
#include "STK600_ATmega128RFA1/BoardSupportPackage.h"
#else
#define BOARDSUPPORTPACKAGE_UNIMPLEMENTED
#endif

#ifndef BOARDSUPPORTPACKAGE_UNIMPLEMENTED

extern void BoardSupportPackage_Initialize();

#endif

#endif /* BOARDSUPPORTPACKAGE_H_ */
