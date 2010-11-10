/*
 * NonVolatileStorage.h
 *
 *  Created on: 12/10/2010
 *      Author: Coma
 */

#ifndef NONVOLATILESTORAGE_H_
#define NONVOLATILESTORAGE_H_

extern void NonVolatileStorage_Read(const void* object, void* buffer, uint8_t length);
extern void NonVolatileStorage_Write(void* object, const void* buffer, uint8_t length);

#endif /* NONVOLATILESTORAGE_H_ */
