/*
 * Network.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <Globals.h>

enum
{
	FLOW_SPONTANEOUS,
	FLOW_CONSTANT,
	FLOW_BURSTY
};

typedef struct
{
	uint8_t id;
	uint32_t bandwidth;
	uint8_t flow;
	uint8_t maxLoss;
} sensor;

enum
{
	CLASSIFICATION_BULK,
	CLASSIFICATION_STREAM,
	CLASSIFICATION_NOTIFICATION
};

void Network_Initialize();
void Network_Run();
uint8_t Network_CreateChannel(uint8_t classification);
void Network_SendData(uint8_t dataId, void* data, uint8_t length);

#endif /* NETWORK_H_ */
