/*
 * Dianostics.c
 *
 *  Created on: 05/11/2010
 *      Author: Coma
 */

#include "Diagnostics.h"
#include "../HardwareAbstractionLayer/DiagnosticsLink.h"

#ifdef DIAGNOSTICS

enum
{
	ESC = 251,
	EOF,
	SOF_STATE,
	SOF_MESSAGE,
	SOF_EVENT
};

static uint8_t AddData(uint8_t value, uint8_t* buffer, uint8_t position);
static void FrameHandler(void* data, uint8_t length);

static uint32_t sn;

void Diagnostics_Initialize(uint32_t serialNumber)
{
	DiagnosticsLink_Initialize(FrameHandler);

	sn = serialNumber;
}

void Diagnostics_SendEvent(uint8_t eventId)
{
	if (sn > 3)
	{
		return;
	}

	uint8_t buffer[1 + 2 + 1];
	uint8_t position = 1; // SOF never need stuffing so skip it

	buffer[0] = SOF_EVENT;
	position += AddData(eventId, buffer, position);
	buffer[position] = EOF;

	DiagnosticsLink_Send(buffer, position + 1);
}

void Diagnostics_SendMessage(uint8_t messageId, void* message, uint8_t messageLength)
{

}

void Diagnostics_SendState(uint8_t stateMachineId, uint8_t newState)
{

}

static uint8_t AddData(uint8_t value, uint8_t* buffer, uint8_t position)
{
	if (value >= ESC)
	{
		buffer[position] = ESC;
		buffer[position + 1] = value & 0x7f;
		return 2;
	}
	else
	{
		buffer[position] = value;
		return 1;
	}
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
