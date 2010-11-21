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

typedef void (*event_handler)(uint8_t event, void* data);

/**
 *
 */
extern void EventDispatcher_Initialize();

/**
 *
 */
extern void EventDispatcher_Dispatch();

/**
 *
 * @param eventId
 * @param handler
 */
extern void EventDispatcher_Subscribe(uint8_t eventId, event_handler handler);

/**
 *
 * @param eventId
 * @param data
 * @return
 */
extern bool EventDispatcher_Publish(uint8_t eventId, void* data);

/**
 *
 * @param handler
 * @return Returns true if the notification was successfully queued.
 */
extern bool EventDispatcher_Notify(completion_handler handler);

/**
 * Enqueues data for deferred processing.
 * @param handler The processing function.
 * @param data The data to be processed.
 * @param length The length of the data.
 */
extern bool EventDispatcher_Process(block_handler handler, void* data, uint8_t length);

#endif /* EVENTDISPATCHER_H_ */
