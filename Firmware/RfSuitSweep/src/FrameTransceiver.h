/*
 * FrameTransceiver.h
 *
 *  Created on: 10/05/2010
 *      Author: coma
 */

#ifndef FRAMETRANSCEIVER_H_
#define FRAMETRANSCEIVER_H_

#include <Globals.h>

#if defined(FRAMETRANSCEIVER_TX_BUFFER_SIZE) || defined(FRAMETRANSCEIVER_RX_BUFFER_SIZE) || defined(FRAMETRANSCEIVER_MAXIMUM_FRAME_SIZE)

#define __FRAMETRANSCEIVER__

typedef struct
{
	uint8_t uart;
	uint16_t ubrr;
	blockHandlerCallback frameHandler;
}frameTransceiver;

extern void FrameTransceiver_Initialize(uint16_t ubrr, blockHandlerCallback frameReceivedHandler);
extern void FrameTransceiver_Send(void* data, uint8_t length);

#endif

#endif /* FRAMETRANSCEIVER_H_ */
