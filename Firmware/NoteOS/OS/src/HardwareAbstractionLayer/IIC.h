/*
 * IIC.h
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

#ifndef IIC_H_
#define IIC_H_

#include "../Globals.h"
#include "../EventSubsystem/EventDispatcher.h"

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/IIC.h"
#else
#define IIC_UNIMPLEMENTED
#endif

#ifndef IIC_UNIMPLEMENTED

extern void IIC_Initialize();
extern void IIC_Subscribe(iic_configuration* configuration);
extern void IIC_Start();

extern void IIC_BeginTransaction(iic_configuration* configuration);
extern void IIC_EndTransaction();
extern void IIC_Read(uint8_t address, uint8_t* data, uint8_t length);
extern void IIC_StartRead(uint8_t address, uint8_t* data, uint8_t length);
extern void IIC_Write(uint8_t address, uint8_t* data, uint8_t length);
extern void IIC_StartWrite(uint8_t address, uint8_t* data, uint8_t length);

#endif

#endif /* IIC_H_ */
