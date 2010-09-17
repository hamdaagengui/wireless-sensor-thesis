/*
 * Queue.h
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include  <Globals.h>

typedef struct
{
	uint8_t in;
	uint8_t out;
	uint8_t elementSize;
	uint8_t elementsFree;
	uint8_t elementsMax;
	uint8_t buffer[];
} queue;

#define Queue_Head(q) ((void*)&(((queue*)q)->buffer[((queue*)q)->out * ((queue*)q)->elementSize]))

extern void Queue_Initialize(void* q, uint8_t elementSize, uint8_t elementCount);
extern void Queue_Put(void* q, void* data);
extern void Queue_Get(void* q, void* data);
extern bool Queue_IsFull(void* q);
extern bool Queue_IsEmpty(void* q);

//extern void* Queue_Head(void* q);
//extern void* Queue_Tail(void* q);
//extern void AdvanceHead(void* q);
//extern void AdvanceTail(void* q);

#endif /* QUEUE_H_ */
