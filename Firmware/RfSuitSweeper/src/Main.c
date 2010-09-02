/*
 * Main.c
 *
 *  Created on: 27/08/2010
 *      Author: Coma
 */

#define BAUDRATE_115200													16
#define BAUDRATE_250000													7
#define BAUDRATE_500000													3

void FrameHandler(uint8_t* data, uint8_t length);

int main()
{
	// Initialize IO ports
	DDRE = 0x1c; // LEDs, button and UART
	PORTE = 0x3c;
	PORTD = 0xaa; // Id configuration
	DDRD = 0x55;

	PORTG = 0x00; // test probe pins
	DDRG = 0xff;


	// Read id [0-15]
	uint8_t sn = 0x00;
	sn += ReadBit(PIND, 1) ? 0 : 1;
	sn += ReadBit(PIND, 3) ? 0 : 2;
	sn += ReadBit(PIND, 5) ? 0 : 4;
	sn += ReadBit(PIND, 7) ? 0 : 8;


	// Initialize kernel
	Kernel_Initialize();

	Rs485Driver_Initialize(BAUDRATE_500000, FrameHandler, TxOn, TxOff);


	// Start the system
	Kernel_Run();

	return 0;
}

void FrameHandler(uint8_t* data, uint8_t length)
{
	adrcmdMessageBase* mb = (adrcmdMessageBase*) data;

	if (mb->destination != id && mb->destination != BROADCAST_ID) // reject frame?
	{
		return;
	}

	bool doSignalActivity = true;

	switch (mb->messageId)
	{
		case MESSAGEID_CONFIGUREACCESSPOINT:
			{
				configureAccessPointMessage* m = (configureAccessPointMessage*) data;
			}
	}
}
