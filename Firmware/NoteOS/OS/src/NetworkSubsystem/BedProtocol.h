/*
 * BedProtocol.h
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#ifndef BEDPROTOCOL_H_
#define BEDPROTOCOL_H_

#include "../Globals.h"

extern void* BedProtocol_CreatePacket(uint8_t receiver, uint8_t type, uint8_t size);
extern bool BedProtocol_QueuePacket();
extern bool BedProtocol_ProcessPacket(void** data, uint8_t length, block_handler packetHandler);

#endif /* BEDPROTOCOL_H_ */
