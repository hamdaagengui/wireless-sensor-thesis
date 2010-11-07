/*
 * DiagnosticsLink.c
 *
 *  Created on: 24/09/2010
 *      Author: Coma
 */

#include "../DiagnosticsLink.h"
#include "../../EventSubsystem/EventDispatcher.h"
#include "../../Collections/FIFO.h"

static uint8_t txFifo[sizeof(fifo) + DIAGNOSTICS_LINK_TRANSMISSION_BUFFER_SIZE];
static uint8_t rxFifo[sizeof(fifo) + DIAGNOSTICS_LINK_RECEPTION_BUFFER_SIZE];

static block_handler frameHandler;

void DiagnosticsLink_Initialize(block_handler frameReceivedHandler)
{
	frameHandler = frameReceivedHandler;

	UCSR0A = 1 << U2X0;
	UCSR0B = 1 << RXCIE0 | 1 << RXEN0 | 1 << TXEN0;
	UCSR0C = 1 << UCSZ01 | 1 << UCSZ00;
	UBRR0 = DIAGNOSTICS_LINK_BAUDRATE;
}

void DiagnosticsLink_Send(void* data, uint8_t length)
{
	Critical();

	FIFO_Write(txFifo, data, length);
	// activate/reactivate
	UCSR0B |= 1 << UDRE0;

	NonCritical();
}

ISR(USART0_RX_vect)
{
	FIFO_WriteByte(rxFifo, UDR0);
}

ISR(USART0_UDRE_vect)
{
	if (FIFO_IsEmpty(txFifo))
	{
		UCSR0B &= ~(1 << UDRE0);
	}
	else
	{
		UDR0 = FIFO_ReadByte(txFifo);
	}
}
