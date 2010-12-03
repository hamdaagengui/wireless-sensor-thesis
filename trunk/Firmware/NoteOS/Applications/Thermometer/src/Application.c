/*
 * Application.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "NoteOS.h"

void ThermoUpdate(sensor_id sensor, void* data)
{
	ApplicationProtocols_SendSensorData(0, 1, data, 2);
}

void Ping()
{
	char pingMsg[] = { "Ping" };
	ApplicationProtocols_SendRawBedData(BROADCAST_ADDRESS, pingMsg, sizeof(pingMsg));

	Leds_RedToggle();
}

timer_configuration tim;

void Synced(event e, void* data)
{
	Leds_YellowOn();
}

void Connected(event e, void* data)
{
	Leds_GreenOn();
}

void Start()
{
	//	SensorManager_InstallSensor(&temperatureSensorMax6662Interface);
	//	SensorManager_InstallSensor(&heartRateVariabilityInterface);
	//
	//	EventDispatcher_Subscribe(SENSOR_TEMPERATURE_EAR_LEFT, ThermoUpdate);

	EventDispatcher_Subscribe(EVENT_SYNCHRONIZED, Synced);
	EventDispatcher_Subscribe(EVENT_CONNECTED, Connected);


#if MASTER_NODE == 1
	Timer_CreateConfiguration(&tim, 1000000, TIMER_MODE_RELAXED_CONTINUES, Ping);
	Timer_Start(&tim);
#endif
}
