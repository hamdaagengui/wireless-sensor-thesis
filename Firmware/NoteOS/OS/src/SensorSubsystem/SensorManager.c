/*
 * SensorManager.c
 *
 *  Created on: 21/10/2010
 *      Author: coma
 */

#include "SensorManager.h"

static const sensor_interface* sensors[SENSOR_MANAGER_MAXIMUM_NUMBER_OF_SENSORS];
static uint8_t numberOfSensorsInstalled;

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

void SensorManager_SetProperty(void* data, uint8_t length)
{
	application_set_request_packet* packet = data;

	uint8_t sensor = packet->sensor;

	if (sensor >= numberOfSensorsInstalled)
	{
		Network_CreateGetResponsePacket(0, PROPERTY_STATUS_INVALID_SENSOR, 0);
		Network_QueueRddPacket();
	}
	else
	{
		sensors[sensor]->set(packet);
	}
}

void SensorManager_GetProperty(void* data, uint8_t length)
{
	application_get_request_packet* packet = data;

	uint8_t sensor = packet->sensor;

	if (sensor >= numberOfSensorsInstalled)
	{
		Network_CreateGetResponsePacket(0, PROPERTY_STATUS_INVALID_SENSOR, 0);
		Network_QueueRddPacket();
	}
	else
	{
		sensors[sensor]->get(packet);
	}
}
