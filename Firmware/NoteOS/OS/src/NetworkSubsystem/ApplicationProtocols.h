/*
 * ApplicationProtocols.h
 *
 *  Created on: 03/12/2010
 *      Author: coma
 */

#ifndef APPLICATIONPROTOCOLS_H_
#define APPLICATIONPROTOCOLS_H_

#include "NetworkInternals.h"
#include "RddProtocol.h"
#include "BedProtocol.h"

extern bool ApplicationProtocols_SendRawBedData(uint8_t receiver, void* data, uint8_t dataSize);
extern bool ApplicationProtocols_SendSensorData(uint8_t receiver, uint8_t sensor, void* data, uint8_t dataSize);
extern bool ApplicationProtocols_SendSetResponse(uint8_t receiver, property_status status);
extern bool ApplicationProtocols_SendGetResponse(uint8_t receiver, property_status status, void* data, uint8_t dataSize);


#endif /* APPLICATIONPROTOCOLS_H_ */
