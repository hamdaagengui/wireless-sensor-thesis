/*
 * Application.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include <SensorSubsystem/SensorManager.h>
#include <SensorSubsystem/Sensors/HeartRateVariability.h>

void Start()
{
	sensor_interface hrv;
	HeartRateVariability_Create(&hrv);
	SensorManager_InstallSensor(&hrv);
}
