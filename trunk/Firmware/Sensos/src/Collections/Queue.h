/*
 * Queue.h
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "../Globals.h"

typedef struct
{
	uint8_t head;
	uint8_t tail;
	uint8_t free;
	uint8_t size;
	uint8_t elementSize;
	uint8_t buffer[];
} queue;

#define Queue_CalculateSize(elementSize, elementCount) (sizeof(queue) + ((elementSize) * (elementCount)))

extern void Queue_Initialize(void* q, uint8_t elementSize, uint8_t elementCount);
extern bool Queue_IsFull(void* q);
extern bool Queue_IsEmpty(void* q);
extern void* Queue_Head(void* q);
extern void* Queue_Tail(void* q);
extern void Queue_AdvanceHead(void* q);
extern void Queue_AdvanceTail(void* q);

#endif /* QUEUE_H_ */
