/*
 * EventDispatcher.h
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#ifndef EVENTDISPATCHER_H_
#define EVENTDISPATCHER_H_

#include "../Globals.h"
#include "../DefaultConfiguration.h"

enum
{
	EVENT_SENSOR_DATA = 128
};

typedef struct
{
	uint8_t id;
	void* data;
} event_report;

typedef struct
{
	uint8_t sensorId;
	uint8_t length;
	uint8_t data[];
} sensor_event_report;

typedef void (*event_handler)(event_report* report);
typedef void (*completion_handler)();

extern void EventDispatcher_Initialize();
extern void EventDispatcher_Dispatch();
extern void EventDispatcher_Subscribe(uint8_t event, event_handler handler);
extern void EventDispatcher_Notify(completion_handler handler);
extern void EventDispatcher_Process(block_handler handler, void* data, uint8_t length);
extern void EventDispatcher_Publish(uint8_t event, void* data);

#endif /* EVENTDISPATCHER_H_ */
