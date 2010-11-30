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

typedef enum
{
	SENSOR_TEMPERATURE_EAR_LEFT = 1000,
	SENSOR_ECG_THREE_LEAD
} sensor_id;

typedef struct
{
	bool (*initialize)(uint8_t id);
	void (*set)(application_set_request_packet* packet);
	void (*get)(application_get_request_packet* packet);
} sensor_interface;

extern bool SensorManager_InstallSensor(const sensor_interface* sensor);
extern void SensorManager_SetProperty(void* data, uint8_t length);
extern void SensorManager_GetProperty(void* data, uint8_t length);

#endif /* SENSORMANAGER_H_ */
