/*
 * IIC.h
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

typedef struct
{
	uint32_t bitrate;
	notification_handler completed;
	notification_handler failed;
} iic_configuration;
