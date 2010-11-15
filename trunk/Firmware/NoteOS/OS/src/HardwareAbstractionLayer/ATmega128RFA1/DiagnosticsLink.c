/*
 * DiagnosticsLink.c
 *
 *  Created on: 24/09/2010
 *      Author: Coma
 */

#include <util/atomic.h>
#include "../DiagnosticsLink.h"
#include "../../Collections/FIFO.h"
#include "../../HardwareAbstractionLayer/HardwareAbstractionLayer.h"

static uint8_t txFifo[sizeof(fifo) + DIAGNOSTICS_LINK_TRANSMISSION_BUFFER_SIZE];
static uint8_t rxFifo[sizeof(fifo) + DIAGNOSTICS_LINK_RECEPTION_BUFFER_SIZE];

static block_handler frameHandler;

void DiagnosticsLink_Initialize(block_handler frameReceivedHandler)
{
	frameHandler = frameReceivedHandler;

	FIFO_Initialize(txFifo, DIAGNOSTICS_LINK_TRANSMISSION_BUFFER_SIZE);
	FIFO_Initialize(rxFifo, DIAGNOSTICS_LINK_RECEPTION_BUFFER_SIZE);

	UCSR0A = 1 << U2X0;
	UCSR0B = 1 << RXCIE0 | 1 << RXEN0 | 1 << TXEN0;
	UCSR0C = 1 << UCSZ01 | 1 << UCSZ00;
	UBRR0 = ((F_CPU / (8L * (DIAGNOSTICS_LINK_BAUDRATE))) - 1L);
}

void DiagnosticsLink_Send(void* data, uint8_t length)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		FIFO_Write(txFifo, data, length);

		SetBit(UCSR0B, UDRIE0);
	}
}

ISR(USART0_RX_vect)
{
	FIFO_WriteByte(rxFifo, UDR0);
	//	uint8_t data = UDR0;
	//	frameHandler(&data, 1);
}

ISR(USART0_UDRE_vect)
{
	if (FIFO_IsEmpty(txFifo))
	{
		ClearBit(UCSR0B, UDRIE0);
	}
	else
	{
		UDR0 = FIFO_ReadByte(txFifo);
	}
}
