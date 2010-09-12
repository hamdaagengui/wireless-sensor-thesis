/*
 * Network.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <Globals.h>

void Network_Initialize();
void Network_Run();
void Network_Send(void* data, uint8_t length);

#endif /* NETWORK_H_ */
