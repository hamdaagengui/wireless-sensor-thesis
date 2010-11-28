/*
 * DiagnosticsLink.h
 *
 *  Created on: 24/09/2010
 *      Author: Coma
 */

#ifndef DIAGNOSTICSLINK_H_
#define DIAGNOSTICSLINK_H_

#include "../Globals.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/DiagnosticsLink.h"
#else
#define DIAGNOSTICSLINK_UNIMPLEMENTED
#endif

#ifndef DIAGNOSTICSLINK_UNIMPLEMENTED

extern void DiagnosticsLink_Initialize(block_handler frameReceivedHandler);
extern void DiagnosticsLink_Send(void* data, uint8_t length);

#endif

#endif /* DIAGNOSTICSLINK_H_ */
