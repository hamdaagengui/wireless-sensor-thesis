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
#include "RadioDriver.h"
#include "RadioMessages.h"

#define BAUDRATE_115200													16
#define BAUDRATE_250000													7
#define BAUDRATE_500000													3

void CableFrameHandler(uint8_t* data, uint8_t length);
void RadioFrameHandler(uint8_t* data, uint8_t length);

uint8_t adr = 1;

uint8_t reports[16] = { 0 };
bool reportReady = false;

uint8_t ledTimer = 0;
static void Run()
{
	if (ledTimer > 0)
	{
		if (--ledTimer == 0)
		{
			SetBit(PORTC, 0);
		}
	}

	Kernel_Sleep(10);
}

int main()
{
	//	// Initialize IO ports
	//	DDRE = 0x1c; // LEDs, button and UART
	//	PORTE = 0x3c;
	//	PORTD = 0xaa; // Id configuration
	//	DDRD = 0x55;
	//
	//	PORTG = 0x00; // test probe pins
	//	DDRG = 0xff;

	DDRE = 0x02;
	PORTE = 0x02;

	DDRC = 0xff;
	PORTC = 0xff;


	// Read id [0-15]
	//	uint8_t sn = 0x00;
	//	sn += ReadBit(PIND, 1) ? 0 : 1;
	//	sn += ReadBit(PIND, 3) ? 0 : 2;
	//	sn += ReadBit(PIND, 5) ? 0 : 4;
	//	sn += ReadBit(PIND, 7) ? 0 : 8;


	// Initialize kernel
	Kernel_Initialize();

	FrameTransceiver_Initialize(BAUDRATE_115200, CableFrameHandler);
	//	RadioDriver_Initialize(RadioFrameHandler);

	Kernel_CreateTask(Run);


	// Start the system
	Kernel_Run();

	return 0;
}

void ShoutAndPass(uint8_t address)
{
	if (address == adr)
	{
		static uint8_t counter = 0;
		if (++counter >= 16)
		{
			counter = 0;
			reportReady = true;
		}


		// shout
		// wait for shout to be out there
		if (reportReady)
		{
			reportReady = false;

			reportTokenMessage m;
			m.destination = address + 1;
			m.source = adr;
			m.messageId = MESSAGEID_REPORTTOKEN;
			for (uint8_t i = 0; i < lengthof(reports); i++)
			{
				m.rssis[i] = reports[i];
			}
			FrameTransceiver_Send(&m, sizeof(m));
		}
		else
		{
			nothingTokenMessage m;
			m.destination = address + 1;
			m.source = adr;
			m.messageId = MESSAGEID_NOTHINGTOKEN;
			FrameTransceiver_Send(&m, sizeof(m));
		}
	}
}

void CableFrameHandler(uint8_t* data, uint8_t length)
{
	bool doLed = true;

	cableMsgMessageBase* mb = AsCableMsgMessageBase(data);
	switch (mb->messageId)
	{
		case MESSAGEID_PINGREQUEST:
			if (mb->destination == adr)
			{
				pingReplyMessage m;
				m.destination = mb->source;
				m.source = adr;
				m.messageId = MESSAGEID_PINGREPLY;
				FrameTransceiver_Send(&m, sizeof(m));
			}
			break;

		case MESSAGEID_NOTHINGTOKEN:
			ShoutAndPass(mb->destination);
			break;

		case MESSAGEID_SETTXPOWERTOKEN:
			// set rf tx power = AsSetTxPowerTokenMessage(data)->txPower
			ShoutAndPass(mb->destination);
			break;

		case MESSAGEID_REPORTTOKEN:
			ShoutAndPass(mb->destination);
			break;

		default:
			doLed = false;
			break;
	}

	if (doLed)
	{
		ClearBit(PORTC, 0);
		ledTimer = 4;
	}
}

void RadioFrameHandler(uint8_t* data, uint8_t length)
{

}
