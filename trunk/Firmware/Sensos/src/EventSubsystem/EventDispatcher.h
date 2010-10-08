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

typedef void (*event_handler)(event_report* report);
typedef void (*completion_handler)();

extern void EventDispatcher_Initialize();
extern void EventDispatcher_Start();
extern void EventDispatcher_RegisterSubscriber(uint8_t event, event_handler handler);
extern void* EventDispatcher_RegisterPublisher(uint8_t id);

extern void EventDispatcher_Dispatch();

extern void* EventDispatcher_Publish(uint8_t event, void* data);
extern void EventDispatcher_Notify(completion_handler handler);

#endif /* EVENTDISPATCHER_H_ */
