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
	CLASSIFICATION_BULK,
	CLASSIFICATION_STREAM,
	CLASSIFICATION_NOTIFICATION
};

void Network_Initialize();
void Network_Run();
uint8_t Network_CreateChannel(uint8_t classification);
void Network_SendSensorData(uint8_t sensorId, void* data, uint8_t length);

#endif /* NETWORK_H_ */
