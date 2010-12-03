/*
 * EventDispatcher.h
 *
 *  Created on: 17/09/2010
 *      Author: Coma
 */

#ifndef EVENTDISPATCHER_H_
#define EVENTDISPATCHER_H_

#include "../Globals.h"
#include <Configuration.h>
#include "../SensorSubsystem/SensorManager.h"

typedef enum
{
	EVENT_SYNCHRONIZED,
	EVENT_CONNECTED,
	EVENT_SENSOR_DATA
} event;

typedef struct
{
	sensor_id sensor;
	uint8_t data[];
} sensor_event;

typedef void (*event_handler)(event e, void* data);

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
extern void EventDispatcher_Subscribe(event e, event_handler handler);

/**
 *
 * @param sensor
 * @param data
 * @return
 */
extern bool EventDispatcher_Publish(event e, void* data);

/**
 *
 * @param handler
 * @return Returns true if the completion was successfully queued.
 */
extern bool EventDispatcher_Complete(completion_handler handler);

/**
 * Enqueues data for deferred processing.
 * @param handler The processing function.
 * @param data The data to be processed.
 * @param length The length of the data.
 */
extern bool EventDispatcher_Process(block_handler handler, void* data, uint8_t length);

#endif /* EVENTDISPATCHER_H_ */
