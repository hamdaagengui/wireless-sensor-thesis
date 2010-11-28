/*
 * Adc.c
 *
 *  Created on: 13/09/2010
 *      Author: coma
 */

#if defined(__AVR_ATmega128RFA1__)

#include <avr/power.h>
#include "../ADC.h"
#include "../../Collections/Queue.h"
#include "../../EventSubsystem/EventDispatcher.h"
#include "../../Diagnostics/Diagnostics.h"

static bool enabled = false;

typedef struct
{
	adc_configuration* configuration;
} operation;
static uint8_t operationQueue[Queue_CalculateSize(sizeof(operation), ADC_OPERATION_QUEUE_SIZE)];
static operation* currentOperation;

static void ExecuteOperation();

void ADC_Initialize()
{
	Queue_Initialize(operationQueue, sizeof(operation), ADC_OPERATION_QUEUE_SIZE);
}

void ADC_Start()
{
	if (enabled)
	{
		//power_spi_enable();


		SetBit(ADCSRA, ADIF); // make sure no interrupt is pending
		SetBit(ADCSRA, ADIE); // enable interrupts

		//Diagnostics_SendEvent(DIAGNOSTICS_ADC_CREATED);
	}
	else
	{
		//Diagnostics_SendEvent(200);
		//power_spi_disable();
	}
}

void ADC_CreateConfiguration(adc_configuration* configuration, uint8_t channel, adc_prescaler prescaler, uint16_t* result, notification_handler completed)
{
	SetBit(DIDR0, channel);

	configuration->admux = 0x40 | channel;
	configuration->adcsra = 1 << ADEN | 1 << ADIE | prescaler;
	configuration->result = result;
	configuration->completed = completed;

	enabled = true;

	Diagnostics_SendEvent(DIAGNOSTICS_ADC_CREATED);
}

bool ADC_Convert(adc_configuration* configuration)
{
	if (Queue_IsFull(operationQueue))
	{
		Diagnostics_SendEvent(DIAGNOSTICS_ADC_FAILED_TO_QUEUE_CONVERSION);
		return false;
	}

	bool isIdle;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		isIdle = Queue_IsEmpty(operationQueue);
	}

	operation* o = Queue_Head(operationQueue);

	o->configuration = configuration;

	Queue_AdvanceHead(operationQueue);

	if (isIdle) // just check some register bit instead of isIdle?
	{
		// power up peripheral
		ExecuteOperation();
	}

	Diagnostics_SendEvent(DIAGNOSTICS_ADC_CONVERSION_QUEUED);

	return true;
}

// Internals

static void ExecuteOperation()
{
	currentOperation = Queue_Tail(operationQueue);

	ADMUX = currentOperation->configuration->admux;
	ADCSRA = currentOperation->configuration->adcsra;

	SetBit(ADCSRA, ADSC);

	Diagnostics_SendEvent(DIAGNOSTICS_ADC_CONVERSION_STARTED);
}

ISR( ADC_vect)
{
	*currentOperation->configuration->result = ADC;
	EventDispatcher_Notify(currentOperation->configuration->completed);

	Queue_AdvanceTail(operationQueue);

	Diagnostics_SendEvent(DIAGNOSTICS_ADC_CONVERSION_COMPLETED);

	if (Queue_IsEmpty(operationQueue))
	{
		// power down peripheral
	}
	else
	{
		ExecuteOperation();
	}
}

#endif
