/*
 * Diagnostics.h
 *
 *  Created on: 05/11/2010
 *      Author: Coma
 */

#ifndef DIAGNOSTICS_H_
#define DIAGNOSTICS_H_

#include "../Globals.h"

extern void Diagnostics_Initialize();
extern void Diagnostics_SendEvent(uint8_t eventId);
extern void Diagnostics_SendMessage(uint8_t messageId, void* message, uint8_t messageLength);
extern void Diagnostics_SendState(uint8_t stateMachineId, uint8_t newState);

#endif /* DIAGNOSTICS_H_ */
