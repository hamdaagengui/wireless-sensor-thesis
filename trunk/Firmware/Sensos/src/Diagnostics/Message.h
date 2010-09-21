/*
 * Message.h
 *
 *  Created on: 15/09/2010
 *      Author: Coma
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "../Globals.h"
#include <avr/pgmspace.h>

#if MESSAGE_LEVEL >= 1
extern void Message_ApplicationMessage(const char* msg);
#else
#define Message_ApplicationMessage(msg)
#endif

#if MESSAGE_LEVEL >= 2
extern void Message_FrameworkMessage(const char* msg);
#else
#define Message_FrameworkMessage(msg)
#endif

#endif /* MESSAGE_H_ */
