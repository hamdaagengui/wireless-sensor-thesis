/*
 * PowerManager.c
 *
 *  Created on: 19/09/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include <avr/sleep.h>
#include "../PowerManager.h"
#include "../../EventSubsystem/EventDispatcher.h"

void PowerManager_Initialize()
{

}

void PowerManager_PowerDown()
{
	return;

	set_sleep_mode(SLEEP_MODE_IDLE); // TODO Could this be more aggressive e.g. using standby?
	cli();
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
}

#endif
