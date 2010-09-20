/*
 * Log.h
 *
 *  Created on: 15/09/2010
 *      Author: Coma
 */

#ifndef LOG_H_
#define LOG_H_

#include "../Globals.h"
#include <avr/pgmspace.h>

#if LOG_LEVEL >= 1
extern void Log_ApplicationMessage(const char* msg);
#else
#define Log_ApplicationMessage(msg)
#endif

#if LOG_LEVEL >= 2
extern void Log_FrameworkMessage(const char* msg);
#else
#define Log_FrameworkMessage(msg)
#endif

#endif /* LOG_H_ */

/*

PC -> Devices
	Send ping
	Send logs true/false
	Send errors true/false
	Send frames true/false
	Retrieve statistics

Devices -> PC
	Report ping reply
	Report log: level message
	Report error: level message
	Report frame: number time source destination payload
	Report statistics: id raw_payload

 */
