/*
 * Network.h
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../Globals.h"
#include "../EventSubsystem/EventDispatcher.h"

enum
{
	FLOW_SPONTANEOUS,
	FLOW_CONSTANT,
	FLOW_BURSTY
};

enum
{
	CLASSIFICATION_BULK,
	CLASSIFICATION_STREAM,
	CLASSIFICATION_NOTIFICATION
};

typedef struct
{
	uint8_t flowType;
	uint32_t bandwidth; // in units of bytes per second
	completionHandler completed;
} networkConfiguration;

typedef struct
{
	uint8_t id;
	uint32_t bandwidth;
	uint8_t flow;
	uint8_t maxLoss;
} sensor;

extern void Network_Initialize();
extern void Network_Subscribe(networkConfiguration* configuration);
extern void Network_Start();

extern void Network_SendData(uint8_t dataId, void* data, uint8_t length);

#endif /* NETWORK_H_ */
