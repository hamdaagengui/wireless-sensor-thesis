/*
 * NonVolatileStorage.c
 *
 *  Created on: 12/10/2010
 *      Author: Coma
 */

#include <avr/eeprom.h>
#include "../NonVolatileStorage.h"

void NonVolatileStorage_Read(const void* object, void* buffer, uint8_t length)
{
	eeprom_read_block(buffer, object, length);
}

void NonVolatileStorage_Write(void* object, const void* buffer, uint8_t length)
{
	eeprom_write_block(buffer, object, length);
}
