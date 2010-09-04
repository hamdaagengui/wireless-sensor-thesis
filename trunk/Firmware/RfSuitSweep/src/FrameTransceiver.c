/*
 * Rs485Driver.c
 *
 *  Created on: 10/05/2010
 *      Author: coma
 */

#include "FrameTransceiver.h"
#include <Config.h>
#include <Kernel/Kernel.h>
#include "Checksum.h"

#define SOF																			255
#define EOF																			254
#define ESC																			253

static BlockHandlerCallback frameHandler;

static void Run();

// Transceiver data

enum
{
	STATE_IDLE,

	STATE_RECEIVING,
	STATE_RECEIVING_ESC,

	STATE_SEND_SOF,
	STATE_SEND_DATA,
	STATE_SEND_DATA_ESC,
	STATE_SEND_EOF,
	STATE_SEND_FINISHING
};
static uint8_t state = STATE_IDLE;

// The queue holding frames to be transmitted
static uint8_t transmitterQueue[FRAMETRANSCEIVER_TX_BUFFER_SIZE];
static uint16_t transmitterQueueIn = 0;
static uint16_t transmitterQueueOut = 0;
static uint16_t transmitterQueueUsed = 0;
static volatile uint8_t transmitterQueueFrameCount = 0;

// The queue holding received frames until the main task gets scheduled
static uint8_t receiverQueue[FRAMETRANSCEIVER_RX_BUFFER_SIZE];
static uint16_t receiverQueueIn = 0;
static uint16_t receiverQueueOut = 0;
static uint16_t receiverQueueUsed = 0;
static volatile uint8_t receiverQueueFrameCount = 0;

void FrameTransceiver_Initialize(uint16_t ubrr, BlockHandlerCallback frameReceivedHandler)
{
	frameHandler = frameReceivedHandler;

	UCSR0A = 1 << U2X0;
	UCSR0B = 1 << RXCIE0 | 1 << RXEN0 | 1 << TXEN0;
	UCSR0A = 1 << UCSZ01 | 1 << UCSZ00;
	UBRR0 = ubrr;

	Kernel_CreateTask(Run);
}

void Run()
{
	if (receiverQueueFrameCount > 0) // dispatch received frames
	{
		uint8_t length = receiverQueue[receiverQueueOut];
		if (++receiverQueueOut >= FRAMETRANSCEIVER_RX_BUFFER_SIZE)
		{
			receiverQueueOut = 0;
		}

		uint8_t frame[FRAMETRANSCEIVER_MAXIMUM_FRAME_SIZE];

		for (uint16_t i = 0; i < length; i++)
		{
			frame[i] = receiverQueue[receiverQueueOut];
			if (++receiverQueueOut >= FRAMETRANSCEIVER_RX_BUFFER_SIZE)
			{
				receiverQueueOut = 0;
			}
		}

		receiverQueueFrameCount--;

		if (Checksum_AdditionChecksum(frame, length) == 0) // valid checksum?
		{
			frameHandler(frame, length - 1); // call back with the frame (not dumping checksum)
		}
	}

	if (state == STATE_IDLE) // is the transmitter idle?
	{
		if (transmitterQueueCount > 0) // any frames waiting to be sent?
		{
			FIFO_Get(transmitterQueue, &transmitterBufferLength, 1); // load transmission buffer with the frame data
			FIFO_Get(transmitterQueue, transmitterBuffer, transmitterBufferLength);

			transmitterBufferIndex = 0; // point to first byte to be send

			state = STATE_SEND_SOF; // tell transmitter to send the SOF (automatically followed by the loaded frame)

			SetBit(UCSR0B, UDRIE0); // enable transmission by enabling the interrupt sub system of the transmitter
		}
	}
}

void FrameTransceiver_Send(void* data, uint8_t length)
{
	if (FIFO_Free(transmitterQueue) >= (1 + length + 1)) // room enough for the frame plus length specifier and checksum?
	{
		uint8_t checksum = Checksum_AdditionChecksum(data, length); // calculate the sum of all bytes
		checksum = (~checksum) + 1; // convert to two's complement

		Critical();

		FIFO_PutByte(transmitterQueue, length + 1); // place the frame (including checksum) in the queue
		FIFO_Put(transmitterQueue, data, length); // send data (checksum is not part of the data => length - 1
		FIFO_PutByte(transmitterQueue, checksum);

		NonCritical();
	}
}

ISR(USART0_RX_vect)
{
	static uint8_t receiverBuffer[FRAMETRANSCEIVER_MAXIMUM_FRAME_SIZE];
	static uint8_t receiverBufferIndex = 0;

	uint8_t udr = UDR0;

	switch (state)
	{
		case STATE_IDLE:
			if (udr == SOF)
			{
				state = STATE_RECEIVING;
				receiverBufferIndex = 0;
			}
			break;

		case STATE_RECEIVING:
			if (udr == SOF)
			{
				receiverBufferIndex = 0;
			}
			else if (udr == ESC)
			{
				state = STATE_RECEIVING_ESC;
			}
			else if (udr == EOF)
			{
				FIFO_Put(receiverQueue, &receiverBufferIndex, 1);
				FIFO_Put(receiverQueue, receiverBuffer, receiverBufferIndex);
				receiverQueueFrameCount++;
				state = STATE_IDLE;
			}
			else
			{
				if (receiverBufferIndex >= FRAMETRANSCEIVER_MAXIMUM_FRAME_SIZE) // rx temp buffer overflow => reset receiver
				{
					state = STATE_IDLE;
				}
				else
				{
					receiverBuffer[receiverBufferIndex++] = udr;
				}
			}
			break;

		case STATE_RECEIVING_ESC:
			if (udr >= ESC) // frame error
			{
				state = STATE_IDLE;
			}
			else
			{
				if (receiverBufferIndex >= FRAMETRANSCEIVER_MAXIMUM_FRAME_SIZE) // rx temp buffer overflow => reset receiver
				{
					state = STATE_IDLE;
				}
				else
				{
					receiverBuffer[receiverBufferIndex++] = udr | 0x80; // un escape value and store it

					state = STATE_RECEIVING;
				}
			}
			break;

		case STATE_SEND_SOF:
		case STATE_SEND_DATA:
		case STATE_SEND_DATA_ESC:
		case STATE_SEND_EOF:
		case STATE_SEND_FINISHING:
			// verify data on wire
			break;

		default:
			state = STATE_IDLE;
			break;
	}
}

// Transmitter part of the transceiver
ISR(USART0_UDRE_vect)
{
	static uint8_t value;

	switch (state)
	{
		case STATE_SEND_SOF:
			{
				UDR0 = SOF;
				state = STATE_SEND_DATA;
			}
			break;

		case STATE_SEND_DATA:
			{
				FIFO_GetByte(transmitterQueue, &value);
				if (value >= ESC)
				{
					UDR0 = ESC;
					state = STATE_SEND_DATA_ESC;
				}
				else
				{
					UDR0 = value;
					if (FIFO_Used(transmitterQueue) == 0)
					{
						state = STATE_SEND_EOF;
					}
				}
			}
			break;

		case STATE_SEND_DATA_ESC:
			{
				UDR0 = value & 0x7f;
				if (FIFO_Used(transmitterQueue) == 0)
				{
					state = STATE_SEND_EOF;
				}
				else
				{
					state = STATE_SEND_DATA;
				}
			}
			break;

		case STATE_SEND_EOF:
			{
				UDR0 = EOF;

				ClearBit(UCSR0B, UDRIE0); // disable UDRE interrupt
				SetBit(UCSR0A, TXC0); // ensure TXC flag is cleared
				SetBit(UCSR0B , TXCIE0); // enable tx complete interrupt

				state = STATE_SEND_FINISHING;
			}
			break;

		default:
			// TODO Reset transmitter
			break;
	}
}

ISR(USART0_TX_vect)
{
	transmitterQueueCount--;

	state = STATE_IDLE;

	ClearBit(UCSR0B, TXCIE0); // disable TXC interrupt
}
