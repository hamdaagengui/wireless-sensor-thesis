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
static uint32_t activeSn = 0xffffffff;

void Diagnostics_Initialize(uint32_t serialNumber)
{
	DiagnosticsLink_Initialize(FrameHandler);

	sn = serialNumber;
}

void Diagnostics_SendEvent(uint8_t event)
{
	//	if (sn != activeSn)
	//	{
	//		return;
	//	}
	if (ReadBit(PIND, 6) != 0)
	{
		return;
	}

	uint8_t buffer[10];
	uint8_t position = 0; // SOF never need stuffing so skip it

	buffer[position++] = SOF_EVENT;
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
	activeSn = ((uint8_t*) data)[0];
}

#else

void Diagnostics_Initialize(uint32_t serialNumber)
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
