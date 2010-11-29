/*
 * SensorManager.h
 *
 *  Created on: 21/10/2010
 *      Author: coma
 */

#ifndef SENSORMANAGER_H_
#define SENSORMANAGER_H_

#include "../Globals.h"
#include "../NetworkSubsystem/Network.h"

typedef struct
{
	bool (*initialize)(uint8_t id);
	bool (*set)(set_request_packet* packet);
	bool (*get)(get_request_packet* packet);
} sensor_interface;

extern void SensorManager_Initialize();
extern bool SensorManager_InstallSensor(const sensor_interface* sensor);
extern bool SensorManager_SetProperty(set_request_packet* packet);
extern bool SensorManager_GetProperty(get_request_packet* packet);

#endif /* SENSORMANAGER_H_ */
