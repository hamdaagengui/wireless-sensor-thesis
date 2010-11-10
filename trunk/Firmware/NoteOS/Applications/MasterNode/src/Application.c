/*
 * Application.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include <EventSubsystem/EventDispatcher.h>

void SensorDataHandler(event_report* e)
{
}

void Start()
{
	EventDispatcher_Subscribe(EVENT_SENSOR_DATA_RECEIVED, SensorDataHandler);
}
