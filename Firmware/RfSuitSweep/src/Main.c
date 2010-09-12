/*
 * Main.c
 *
 *  Created on: 27/08/2010
 *      Author: Coma
 */

#include <Globals.h>
#include <Kernel/Kernel.h>
#include <FrameTransceiver.h>
#include "CableMessages.h"
#include <Peripherals/RadioDriver.h>
#include "RadioMessages.h"
#include <util/delay.h>

#define BAUDRATE_115200													16
#define BAUDRATE_250000													7
#define BAUDRATE_500000													3

void CableFrameHandler(uint8_t* data, uint8_t length);
void RadioFrameHandler(uint8_t* data, uint8_t length);

uint8_t localAddress = 1;

uint8_t rssis[16] = { 0 };
bool hasFullSweep = false;

uint8_t ledTimer = 0;
static void Run()
{
	if (ledTimer > 0)
	{
		if (--ledTimer == 0)
		{
			SetBit(PORTE, 2);
		}
	}

	Kernel_Sleep(10);
}

void BlinkTrigger()
{
	ToggleBit(PORTE, 3);

	uint8_t data[20];
	data[0] = 123;
	RadioDriver_Send(data, 1);

	Kernel_Sleep(500);
}

int main()
{
	// Initialize IO ports
	DDRE = 0x1c; // LEDs, button and UART
	PORTE = 0x3c;

	PORTD = 0b10101010; // Id configuration
	DDRD = 0b01010101;

	PORTF = 0x00; // test probe pins
	DDRF = 0xff;

	PORTF = 0xa5;

	while (1)
		;

	// Read address (1 - 16)
	localAddress += ReadBit(PIND, 1) ? 0 : 1;
	localAddress += ReadBit(PIND, 3) ? 0 : 2;
	localAddress += ReadBit(PIND, 5) ? 0 : 4;
	localAddress += ReadBit(PIND, 7) ? 0 : 8;


	// Initialize kernel
	Kernel_Initialize();


	// Initialize sub systems
	FrameTransceiver_Initialize(BAUDRATE_115200, CableFrameHandler);

	RadioDriver_Initialize(RadioFrameHandler);
	RadioDriver_SetBitRate(RADIODRIVER_BITRATE_250_KBPS);
	RadioDriver_SetChannel(RADIODRIVER_CHANNEL_11);
	RadioDriver_SetReceiverSensitivityThreshold(RADIODRIVER_RECEIVER_SENSITIVITY_THRESHOLD_DISABLE);
	RadioDriver_SetTxPower(RADIODRIVER_TX_POWER_MAXIMUM);

	Kernel_CreateTask(Run);

	Kernel_CreateTask(BlinkTrigger);


	// Start the system
	Kernel_Run();

	return 0;
}

void ShoutAndPass()
{
	// shout
	ClearBit(PORTE, 3);

	{
		shoutMessage m;
		m.messageId = MESSAGEID_SHOUT;
		m.source = localAddress;
		RadioDriver_Send(&m, sizeof(m)); // shout
	}

	RadioDriver_WaitForSendToComplete(); // wait for shouting to complete...

	SetBit(PORTE, 3);


	// and pass
	//	if (hasFullSweep)
	{
		reportTokenMessage m;
		m.destination = localAddress + 1;
		m.source = localAddress;
		m.messageId = MESSAGEID_REPORTTOKEN;
		for (uint8_t i = 0; i < lengthof(rssis); i++)
		{
			m.rssis[i] = rssis[i];
			rssis[i] = 0;
		}
		FrameTransceiver_Send(&m, sizeof(m));
	}
	//	else
	//	{
	//		nothingTokenMessage m;
	//		m.destination = localAddress + 1;
	//		m.source = localAddress;
	//		m.messageId = MESSAGEID_NOTHINGTOKEN;
	//		FrameTransceiver_Send(&m, sizeof(m));
	//
	//		hasFullSweep = true; // once the first shout has been sent switch to hasFullSweep mode and start reporting RSSIs.
	//	}
}

void CableFrameHandler(uint8_t* data, uint8_t length)
{
	bool doLed = true;

	cableMsgMessageBase* mb = AsCableMsgMessageBase(data);
	switch (mb->messageId)
	{
		case MESSAGEID_PINGREQUEST:
			if (mb->destination == localAddress)
			{
				pingReplyMessage m;
				m.destination = mb->source;
				m.source = localAddress;
				m.messageId = MESSAGEID_PINGREPLY;
				FrameTransceiver_Send(&m, sizeof(m));
			}
			break;

		case MESSAGEID_NOTHINGTOKEN:
			if (mb->destination == localAddress)
			{
				ShoutAndPass();
			}
			break;

		case MESSAGEID_SETCHANNELTOKEN:
			RadioDriver_SetChannel(AsSetChannelTokenMessage(data)->channel);
			if (mb->destination == localAddress)
			{
				ShoutAndPass();
			}
			break;

		case MESSAGEID_SETTXPOWERTOKEN:
			RadioDriver_SetTxPower(AsSetTxPowerTokenMessage(data)->txPower);
			if (mb->destination == localAddress)
			{
				ShoutAndPass();
			}
			break;

		case MESSAGEID_REPORTTOKEN:
			if (mb->destination == localAddress)
			{
				ShoutAndPass();
			}
			break;

		default:
			doLed = false;
			break;
	}

	if (doLed)
	{
		ClearBit(PORTE, 2);
		ledTimer = 4;
	}
}

void RadioFrameHandler(uint8_t* data, uint8_t length)
{
	radioMsgMessageBase* mb = AsRadioMsgMessageBase(data);

	switch (mb->messageId)
	{
		case MESSAGEID_SHOUT:
			ToggleBit(PORTE, 4);
			rssis[mb->source - 1] = RadioDriver_GetRssi();
			break;
	}
}
