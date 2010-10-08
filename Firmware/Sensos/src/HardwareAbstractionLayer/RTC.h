/*
 * RTC.h
 *
 *  Created on: 29/09/2010
 *      Author: coma
 */

#ifndef RTC_H_
#define RTC_H_

#include "../Globals.h"
#include "../DefaultConfiguration.h"

typedef struct
{
	uint32_t bitrate;
	uint8_t csPin;
	completion_handler completed;
} rtc_configuration;

#ifdef __AVR_ATmega128RFA1__
#include "ATmega128RFA1/RTC.h"
#else
#error "No RTC driver found for the selected processor!"
#endif

extern void RTC_Initialize();
extern void RTC_Subscribe(rtc_configuration* configuration);
extern void RTC_Start();

/**
 * Queues a transfer command. OBS Only a pointer to the output buffer is queued so it is not safe to modify the buffer until the completion handler is called!
 */
extern void RTC_Request(spi_configuration* configuration, uint8_t* output, uint8_t* input, uint8_t length);

#endif /* RTC_H_ */jeg
