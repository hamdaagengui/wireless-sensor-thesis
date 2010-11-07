/*
 * Network.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../Globals.h"

extern void Network_Initialize(bool master);
extern void Network_SetId(uint8_t id);
extern void Network_SetDataHandler(block_handler handler);
extern bool Network_SendData(uint8_t receiver, void* data, uint8_t length);

extern void Network_TimerEvent();

#endif /* NETWORK_H_ */
