/*
 * Dianostics.c
 *
 *  Created on: 05/11/2010
 *      Author: Coma
 */

#include "Diagnostics.h"
#include "../HardwareAbstractionLayer/DiagnosticsLink.h"

#ifdef DIAGNOSTICS

static void FrameHandler(void* data, uint8_t length);

void Diagnostics_Initialize()
{
	DiagnosticsLink_Initialize(1, FrameHandler);
}

void Diagnostics_SendEvent(uint8_t eventId)
{

}

void Diagnostics_SendMessage(uint8_t messageId, void* message, uint8_t messageLength)
{

}

void Diagnostics_SendState(uint8_t stateMachineId, uint8_t newState)
{

}

static void FrameHandler(void* data, uint8_t length)
{

}

#else

void Diagnostics_Initialize()
{
}

void Diagnostics_SendEvent(uint8_t eventId)
{
}

void Diagnostics_SendMessage(uint8_t messageId, void* message, uint8_t messageLength)
{
}

void Diagnostics_SendState(uint8_t stateMachineId, uint8_t newState)
{
}

#endif
