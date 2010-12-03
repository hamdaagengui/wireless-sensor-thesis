/*
 * Dianostics.c
 *
 *  Created on: 05/11/2010
 *      Author: Coma
 */

#include "Diagnostics.h"
#include "../HardwareAbstractionLayer/DiagnosticsLink.h"
#include "../EventSubsystem/Timer.h"

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

void Diagnostics_Initialize()
{
	DiagnosticsLink_Initialize(FrameHandler);
}

void Diagnostics_SendRaw(uint8_t value)
{
	//	DiagnosticsLink_Send(&value, 1);
}

void Diagnostics_SendEvent(uint8_t event)
{
	if (ReadBit(PIND, 6) != 0)
	{
		return;
	}

//	uint32_t time = Timer_GetLocalTime();

	uint8_t buffer[10];
	uint8_t position = 0; // SOF never need stuffing so skip it

	buffer[position++] = SOF_EVENT;
//	position += AddData(time & 0xff, buffer, position);
//	position += AddData((time >> 8) & 0xff, buffer, position);
//	position += AddData((time >> 16) & 0xff, buffer, position);
//	position += AddData((time >> 24) & 0xff, buffer, position);
	position += AddData(event, buffer, position);
	buffer[position++] = EOF;

	DiagnosticsLink_Send(buffer, position);
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
