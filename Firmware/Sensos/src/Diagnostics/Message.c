/*
 * Message.c
 *
 *  Created on: 15/09/2010
 *      Author: Coma
 */

#include "Message.h"

#if MESSAGE_LEVEL >= 1
void Message_ApplicationMessage(const char* msg)
{

}
#endif

#if MESSAGE_LEVEL >= 2
void Message_FrameworkMessage(const char* msg)
{

}
#endif
