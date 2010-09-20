/*
 * Log.c
 *
 *  Created on: 15/09/2010
 *      Author: Coma
 */

#include "Log.h"

#if LOG_LEVEL >= 1
void Log_ApplicationMessage(const char* msg)
{

}
#endif

#if LOG_LEVEL >= 2
void Log_FrameworkMessage(const char* msg)
{

}
#endif
