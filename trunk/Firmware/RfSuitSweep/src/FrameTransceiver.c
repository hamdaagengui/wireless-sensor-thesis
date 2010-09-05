/*
 * FrameTransceiver.c
 *
 *  Created on: 10/05/2010
 *      Author: coma
 */

#include "FrameTransceiver.h"
#include <Config.h>
#include <Kernel/Kernel.h>

#define SOF																			255
#define EOF																			254
#define ESC																			253

static blockHandlerCallback frameHandler;

static void Run();

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
static uint16_t transmitterQueueFree = FRAMETRANSCEIVER_TX_BUFFER_SIZE;
static uint8_t transmitterQueueFrameCount = 0;

static uint8_t frameLength; // length of frame currently in transmission (shared between the UDRE and TXC interrupts)

// The queue holding received frames until the main task gets scheduled
static uint8_t receiverQueue[FRAMETRANSCEIVER_RX_BUFFER_SIZE];
static uint16_t receiverQueueIn = 0;
static uint16_t receiverQueueOut = 0;
static uint8_t receiverQueueFrameCount = 0;

void FrameTransceiver_Initialize(uint16_t ubrr, blockHandlerCallback frameReceivedHandler)
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

		uint8_t checksum = 0;
		for (uint16_t i = 0; i < length; i++)
		{
			uint8_t value = receiverQueue[receiverQueueOut];

			checksum += value;

			frame[i] = value;
			if (++receiverQueueOut >= FRAMETRANSCEIVER_RX_BUFFER_SIZE)
			{
				receiverQueueOut = 0;
			}
		}

		atomic(receiverQueueFrameCount--);

		if (checksum == 0) // valid checksum?

		{
			frameHandler(frame, length - 1); // call back with the frame (dumping checksum)
		}
	}

	Critical();

	if (state == STATE_IDLE) // is the transmitter idle?

	{
		if (transmitterQueueFrameCount > 0) // any frames waiting to be sent?

		{
			state = STATE_SEND_SOF; // tell transmitter to start sending a frame

			SetBit(UCSR0B, UDRIE0); // enable transmission by enabling the transmitter interrupt
		}
	}

	NonCritical();
}

void FrameTransceiver_Send(void* data, uint8_t length)
{
	if (length > (255 - 1)) // frames (including the checksum) in the buffer can not exceed what can be specified by an uint8_t
	{
		return;
	}

	uint8_t spaceRequired = 1 + length + 1; // total buffer requirements are: length specifier (1), data (<= 254) and checksum (1)?

	if (spaceRequired > transmitterQueueFree)
	{
		return;
	}

	transmitterQueue[transmitterQueueIn] = length + 1; // add length specifier to buffer (length of frame in buffer is length of data plus one for checksum)
	if (++transmitterQueueIn >= FRAMETRANSCEIVER_TX_BUFFER_SIZE)
	{
		transmitterQueueIn = 0;
	}

	uint8_t checksum = 0;
	uint8_t* d = (uint8_t*) data;
	for (uint8_t i = 0; i < length; i++)
	{
		uint8_t value = d[i];

		checksum += value; // calculate checksum

		transmitterQueue[transmitterQueueIn] = value; // add data
		if (++transmitterQueueIn >= FRAMETRANSCEIVER_TX_BUFFER_SIZE)
		{
			transmitterQueueIn = 0;
		}
	}

	checksum = (~checksum) + 1; // convert to two's complement
	transmitterQueue[transmitterQueueIn] = checksum;
	if (++transmitterQueueIn >= FRAMETRANSCEIVER_TX_BUFFER_SIZE)
	{
		transmitterQueueIn = 0;
	}

	Critical();

	transmitterQueueFree -= 1 + length + 1;

	transmitterQueueFrameCount++;

	NonCritical();
}

ISR(USART0_RX_vect)
{
	static uint16_t receptionLengthPosition;
	static uint16_t receptionQueueInPosition;
	static uint8_t receptionFrameLength;

	uint8_t udr = UDR0;

	switch (state)
	{
		case STATE_IDLE:
			if (udr == SOF)
			{
				state = STATE_RECEIVING;
				receptionLengthPosition = receiverQueueIn;
				receptionQueueInPosition = receiverQueueIn;
				if (++receptionQueueInPosition == receiverQueueOut) // did the receiver queue just get full (=> overflow)?
				{
					state = STATE_IDLE;
				}
				else
				{
					receptionFrameLength = 0;
				}
			}
			break;

		case STATE_RECEIVING:
			if (udr == ESC)
			{
				state = STATE_RECEIVING_ESC;
			}
			else if (udr == SOF)
			{
				receptionQueueInPosition = receiverQueueIn;
				if (++receptionQueueInPosition == receiverQueueOut) // did the receiver queue just get full (=> overflow)?
				{
					state = STATE_IDLE;
				}
				else
				{
					receptionFrameLength = 0;
				}
			}
			else if (udr == EOF)
			{
				receiverQueue[receptionLengthPosition] = receptionFrameLength; // store length of received frame
				receiverQueueIn = receptionQueueInPosition; // update new actual position of the in pointer
				receiverQueueFrameCount++;
				state = STATE_IDLE;
			}
			else
			{
				receiverQueue[receptionQueueInPosition] = udr;

				if (++receptionQueueInPosition == receiverQueueOut) // did the receiver queue just get full (=> overflow)?
				{
					state = STATE_IDLE;
				}
				else
				{
					receptionFrameLength++;
				}
			}
			break;

		case STATE_RECEIVING_ESC:
			if (udr == SOF)
			{
				state = STATE_RECEIVING;
				receptionQueueInPosition = receiverQueueIn;
				if (++receptionQueueInPosition == receiverQueueOut) // did the receiver queue just get full (=> overflow)?
				{
					state = STATE_IDLE;
				}
				else
				{
					receptionFrameLength = 0;
				}
			}
			else if (udr >= ESC) // frame error
			{
				state = STATE_IDLE;
			}
			else
			{
				receiverQueue[receptionQueueInPosition] = udr | 0x80; // un escape value and store it

				if (++receptionQueueInPosition == receiverQueueOut) // did the receiver queue just get full (=> overflow)?
				{
					state = STATE_IDLE;
				}
				else // nope - all is well
				{
					state = STATE_RECEIVING;
					receptionFrameLength++;
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
	static uint8_t bytesRemaining;
	static uint8_t value;

	switch (state)
	{
		case STATE_SEND_SOF:
			{
				UDR0 = SOF;

				frameLength = transmitterQueue[transmitterQueueOut]; // load frame length
				bytesRemaining = frameLength;
				if (++transmitterQueueOut >= FRAMETRANSCEIVER_TX_BUFFER_SIZE) // remove length specifier from buffer
				{
					transmitterQueueOut = 0;
				}

				state = STATE_SEND_DATA;
			}
			break;

		case STATE_SEND_DATA:
			{
				value = transmitterQueue[transmitterQueueOut]; // load next byte from buffer
				if (++transmitterQueueOut >= FRAMETRANSCEIVER_TX_BUFFER_SIZE) // remove data from buffer
				{
					transmitterQueueOut = 0;
				}
				bytesRemaining--;

				if (value >= ESC)
				{
					UDR0 = ESC;
					state = STATE_SEND_DATA_ESC;
				}
				else
				{
					UDR0 = value;
					if (bytesRemaining == 0)
					{
						state = STATE_SEND_EOF;
					}
				}
			}
			break;

		case STATE_SEND_DATA_ESC:
			{
				UDR0 = value & 0x7f;
				if (bytesRemaining == 0)
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
	transmitterQueueFree += 1 + frameLength; // free up space from the buffer and its length specifier
	transmitterQueueFrameCount--; // remove the frame from the queue

	state = STATE_IDLE;

	ClearBit(UCSR0B, TXCIE0); // disable TXC interrupt
}
