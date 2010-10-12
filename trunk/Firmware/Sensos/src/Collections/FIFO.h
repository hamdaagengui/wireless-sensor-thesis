/*
 * FIFO.h
 *
 *  Created on: 03/10/2010
 *      Author: coma
 */

#ifndef FIFO_H_
#define FIFO_H_

// TODO This should maybe use pointers instead of indexes to make it faster

#include "../Globals.h"

typedef struct
{
	uint8_t head;
	uint8_t tail;
	uint8_t free;
	uint8_t size;
	uint8_t buffer[];
} fifo;

#define FIFO_CalculateSize(byteCount) (sizeof(fifo) + byteCount)

extern void FIFO_Initialize(void* f, uint8_t byteCount);
extern bool FIFO_IsEmpty(void* f);
extern uint8_t FIFO_GetFreeSpace(void* f);
extern uint8_t FIFO_PeekFirst(void* f);
extern bool FIFO_WriteByte(void* f, uint8_t value);
extern bool FIFO_Write(void* f, void* buffer, uint8_t length);
extern bool FIFO_Read(void* f, void* buffer, uint8_t length);

#endif /* FIFO_H_ */
