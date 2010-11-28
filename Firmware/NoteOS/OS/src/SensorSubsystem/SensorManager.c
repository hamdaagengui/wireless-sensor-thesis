/*
 * SensorManager.c
 *
 *  Created on: 21/10/2010
 *      Author: coma
 */

#include "SensorManager.h"

static const sensor_interface* sensors[SENSOR_MANAGER_MAXIMUM_NUMBER_OF_SENSORS];
static uint8_t numberOfSensorsInstalled;

void SensorManager_Initialize()
{

}

bool SensorManager_InstallSensor(const sensor_interface* sensor)
{
	if (numberOfSensorsInstalled < SENSOR_MANAGER_MAXIMUM_NUMBER_OF_SENSORS)
	{
		if (sensor->initialize(numberOfSensorsInstalled))
		{
			sensors[numberOfSensorsInstalled++] = sensor;
			return true;
		}
	}

	return false;
}

bool SensorManager_SetProperty(set_request_packet* packet)
{
	uint8_t sensor = packet->sensor;

	if (sensor >= numberOfSensorsInstalled)
	{
		return false;
	}

	return sensors[sensor]->set(packet);
}

bool SensorManager_GetProperty(get_request_packet* packet)
{
	uint8_t sensor = packet->sensor;

	if (sensor >= numberOfSensorsInstalled)
	{
		return false;
	}

	return sensors[sensor]->get(packet);
}
