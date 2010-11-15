/*
 * DiagnosticsLink.h
 *
 *  Created on: 24/09/2010
 *      Author: Coma
 */

#ifndef DIAGNOSTICSLINK_H_
#define DIAGNOSTICSLINK_H_

#include "../Globals.h"

extern void DiagnosticsLink_Initialize(block_handler frameReceivedHandler);
extern void DiagnosticsLink_Send(void* data, uint8_t length);

#endif /* DIAGNOSTICSLINK_H_ */
