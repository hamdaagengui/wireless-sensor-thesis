/*
 * IIC.h
 *
 *  Created on: 16/09/2010
 *      Author: Coma
 */

typedef struct
{
	uint32_t bitrate;
	completion_handler completed;
	completion_handler failed;
} iic_configuration;
