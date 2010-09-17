/*
 * EventDispatcher.h
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#ifndef EVENTDISPATCHER_H_
#define EVENTDISPATCHER_H_

#include <Globals.h>
#include <DefaultConfiguration.h>

typedef struct
{
	uint8_t id;
	void* data;
} eventReport;

typedef void (*eventHandler)(eventReport* report);

extern void EventDispatcher_Initialize();
extern void EventDispatcher_Start();
extern void EventDispatcher_ProcessEvents();
extern void EventDispatcher_RegisterSubscriber(uint8_t event, eventHandler handler);
extern void* EventDispatcher_RegisterPublisher(uint8_t id);
extern void* EventDispatcher_Publish(uint8_t event, void* data);
extern void EventDispatcher_Notify(eventHandler handler);

#endif /* EVENTDISPATCHER_H_ */
