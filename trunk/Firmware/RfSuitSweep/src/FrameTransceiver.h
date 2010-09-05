/*
 * FrameTransceiver.h
 *
 *  Created on: 10/05/2010
 *      Author: coma
 */

#ifndef FRAMETRANSCEIVER_H_
#define FRAMETRANSCEIVER_H_

#include <Globals.h>

extern void FrameTransceiver_Initialize(uint16_t ubrr, blockHandlerCallback frameReceivedHandler);
extern void FrameTransceiver_Send(void* data, uint8_t length);

#endif /* FRAMETRANSCEIVER_H_ */
