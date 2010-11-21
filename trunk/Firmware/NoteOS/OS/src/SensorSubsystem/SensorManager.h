/*
 * SensorManager.h
 *
 *  Created on: 21/10/2010
 *      Author: coma
 */

#ifndef SENSORMANAGER_H_
#define SENSORMANAGER_H_

#include "../Globals.h"

typedef bool (*property_invokation)(uint8_t propertyId, void* value);

typedef struct
{
	completion_handler initialize;
	completion_handler reset;
	property_invokation set;
	property_invokation get;
} sensor_interface;

extern void SensorManager_Initialize();
extern void SensorManager_InstallSensor(sensor_interface* sensor);
extern void SensorManager_SetProperty(uint8_t sensorId, uint8_t propertyId, void* value);
extern void SensorManager_GetProperty(uint8_t sensorId, uint8_t propertyId, void* value);

#endif /* SENSORMANAGER_H_ */
