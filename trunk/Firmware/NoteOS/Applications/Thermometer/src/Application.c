/*
 * Application.c
 *
 *  Created on: 12/09/2010
 *      Author: coma
 */

#include "NoteOS.h"

#if MASTER_NODE == 1

char pingMsg[] =
{	"Hello World!"};
void SynchronizationInviter()
{
	ApplicationProtocols_SendRawBedData(14, pingMsg, sizeof(pingMsg));

	Leds_GreenToggle();
}

void BedData(void* data, uint8_t length)
{
	Leds_YellowToggle();
}

timer_configuration synchronizationInvitationTimer;
void Start()
{
	Timer_CreateConfiguration(&synchronizationInvitationTimer, 1000000, TIMER_MODE_RELAXED_CONTINUES, SynchronizationInviter);
	Timer_Start(&synchronizationInvitationTimer);

	Network_SetRawBedDataHandler(BedData);
}

#else

timer_configuration connectionTimer;
timer_configuration pingerTimer;
char pingMsg[] = { "Hello World!" };

void Pinger()
{
	ApplicationProtocols_SendRawBedData(GATEWAY_ADDRESS, pingMsg, sizeof(pingMsg));

	Leds_RedToggle();
}

void BedData(void* data, uint8_t length)
{
	Leds_YellowToggle();
}

void ConnectionBlinker()
{
	Leds_GreenToggle();
}

void Connected(event e, void* data)
{
	Timer_Stop(&connectionTimer);
	Leds_GreenOn();

	Timer_CreateConfiguration(&pingerTimer, 2000000, TIMER_MODE_RELAXED_CONTINUES, Pinger);
	Timer_Start(&pingerTimer);
}

void Start()
{
	EventDispatcher_Subscribe(EVENT_CONNECTED, Connected);

	Network_SetRawBedDataHandler(BedData);

	Timer_CreateConfiguration(&connectionTimer, 80000, TIMER_MODE_RELAXED_CONTINUES, ConnectionBlinker);
	Timer_Start(&connectionTimer);
}

#endif
