/*
 * Iic.h
 *
 *  Created on: 14/09/2010
 *      Author: coma
 */

#ifndef IIC_H_
#define IIC_H_

#include <Globals.h>

typedef struct
{
} iicEvent;

typedef struct
{
	void (*handler)(iicEvent* event);
} iicConfiguration;

extern void Iic_Initialize();
extern void Iic_Subscribe(iicConfiguration* configuration);
extern void Iic_Start();

extern void Iic_EnqueueRead(uint8_t device, uint16_t address, void* data, uint8_t length, eventHandler handler);
extern void Iic_EnqueueContinuedRead(uint8_t device, void* data, uint8_t length, eventHandler handler);
extern void Iic_EnqueueWrite(uint8_t device, uint16_t address, void* data, uint8_t length, eventHandler handler);
extern void Iic_EnqueueContinuedWrite(uint8_t device, void* data, uint8_t length, eventHandler handler);

#endif /* IIC_H_ */
