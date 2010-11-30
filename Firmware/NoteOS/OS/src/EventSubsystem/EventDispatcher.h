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
#include "../SensorSubsystem/SensorManager.h"

typedef void (*event_handler)(sensor_id sensor, void* data);

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
 * @param sensor
 * @param data
 */
extern void EventDispatcher_Subscribe(sensor_id sensor, event_handler handler);

/**
 *
 * @param sensor
 * @param data
 * @return
 */
extern bool EventDispatcher_Publish(sensor_id sensor, void* data);

/**
 *
 * @param handler
 * @return Returns true if the notification was successfully queued.
 */
extern bool EventDispatcher_Notify(notification_handler handler);

/**
 *
 */
//extern bool EventDispatcher_Complete(completion_handler handler, void* operation);

/**
 * Enqueues data for deferred processing.
 * @param handler The processing function.
 * @param data The data to be processed.
 * @param length The length of the data.
 */
extern bool EventDispatcher_Process(block_handler handler, void* data, uint8_t length);

#endif /* EVENTDISPATCHER_H_ */
