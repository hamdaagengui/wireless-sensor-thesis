/*
 * RddProtocol.h
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#ifndef RDDPROTOCOL_H_
#define RDDPROTOCOL_H_

#include "NetworkInternals.h"

extern void RddProtocol_Initialize();
extern void* RddProtocol_CreatePacket(uint8_t receiver, uint8_t type, uint8_t size);
extern void RddProtocol_QueuePacket();
extern void RddProtocol_AcknowledgePacket(link_network_rdd_header* packet);
extern bool RddProtocol_ProcessPacket(void** data, uint8_t length, block_handler packetHandler);
extern void RddProtocol_TimeoutHandler();

#endif /* RDDPROTOCOL_H_ */
