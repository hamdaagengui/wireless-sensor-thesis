/*
 * ApplicationProtocols.c
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#include "ApplicationProtocols.h"
#include <string.h>

bool ApplicationProtocols_SendRawBedData(uint8_t receiver, void* data, uint8_t dataSize)
{
	application_raw_bed_packet* p = BedProtocol_CreatePacket(receiver, TYPE_APPLICATION_RAW_BED, sizeof(application_raw_bed_packet) + dataSize);
	if (p == NULL)
	{
		return false;
	}

	memcpy(p->data, data, dataSize);

	return BedProtocol_QueuePacket();
}

bool ApplicationProtocols_SendSensorData(uint8_t receiver, uint8_t sensor, void* data, uint8_t dataSize)
{
	application_sensor_data_packet* p = RddProtocol_CreatePacket(receiver, TYPE_APPLICATION_SENSOR_DATA, sizeof(application_sensor_data_packet) + dataSize);
	if (p == NULL)
	{
		return false;
	}

	p->sensor = sensor;

	memcpy(p->data, data, dataSize);

	RddProtocol_QueuePacket();

	return true;
}

bool ApplicationProtocols_SendSetResponse(uint8_t receiver, property_status status)
{
	application_set_response_packet* p = RddProtocol_CreatePacket(receiver, TYPE_APPLICATION_SET_PROPERTY_RESPONSE, sizeof(application_set_response_packet));
	if (p == NULL)
	{
		return false;
	}

	p->status = status;

	RddProtocol_QueuePacket();

	return true;
}

bool ApplicationProtocols_SendGetResponse(uint8_t receiver, property_status status, void* data, uint8_t dataSize)
{
	application_get_response_packet* p = RddProtocol_CreatePacket(receiver, TYPE_APPLICATION_GET_PROPERTY_RESPONSE, sizeof(application_get_response_packet) + dataSize);
	if (p == NULL)
	{
		return false;
	}

	p->status = status;

	memcpy(p->data, data, dataSize);

	RddProtocol_QueuePacket();

	return true;
}
