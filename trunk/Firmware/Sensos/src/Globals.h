#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>
#include "DefaultConfiguration.h"

#define ReadBit(var, bit)												(((var) & (1 << (bit))) != 0)
#define SetBit(var, bit)												((var) |= (1 << (bit)))
#define ClearBit(var, bit)											((var) &= ~(1 << (bit)))
#define ToggleBit(var, bit)											((var) ^= (1 << (bit)))

#define Modify(var, mask, value)								(var = ((var & ~mask) | (value & mask)))

#define false																		(0)
#define true																		(1)
typedef uint8_t bool;

//#define NULL																		(0)

#define atomic(action)													do {Critical(); action ; NonCritical();} while(0)

#define Critical()															cli()
#define NonCritical()														sei()
#define Nop()																		__asm__ volatile("nop")
#define lengthof(x)															( sizeof(x) / sizeof(x[0]) )
//#define offsetof(type, member)									__builtin_offsetof (type, member)

#define MemoryBarrier()													__asm__ volatile ("" : : : "memory")

typedef void (*block_handler)(void* data, uint8_t length);
typedef void (*notification_handler)();

#define LA(x) {uint8_t r=0; r |= x&(1<<0)?(1<<0):0; r |= x&(1<<1)?(1<<2):0; r |= x&(1<<2)?(1<<4):0; r |= x&(1<<3)?(1<<6):0; PORTB = r;}

#define NodeInspector_Send(x) if(USE_NODE_INSPECTOR > 0) { while(!(UCSR0A & (1<<UDRE0))); UDR0 = x; }
#define com(x) if(USE_COM > 0) { while(!(UCSR0A & (1<<UDRE0))); UDR0 = x; }

enum
{
	NODE_INSPECTOR_EVENT_SYSTEM_INITIALIZED,
	NODE_INSPECTOR_ALLOCATED_NETWORK_BLOCK,
	NODE_INSPECTOR_FAILED_TO_ALLOCATE_NETWORK_BLOCK,
	NODE_INSPECTOR_RELEASED_NETWORK_BLOCK,
	NODE_INSPECTOR_ALLOCATED_SENSOR_BLOCK,
	NODE_INSPECTOR_FAILED_TO_ALLOCATE_SENSOR_BLOCK,
	NODE_INSPECTOR_RELEASE_SENSOR_BLOCK,
	NODE_INSPECTOR_NOTIFY_QUEUED,
	NODE_INSPECTOR_NOTIFY_EXECUTED,
	NODE_INSPECTOR_PROCESS_QUEUED,
	NODE_INSPECTOR_PROCESS_EXECUTED,
	NODE_INSPECTOR_PUBLISH_QUEUED,
	NODE_INSPECTOR_PUBLISH_EXECUTED,
	NODE_INSPECTOR_RX_RTS,
	NODE_INSPECTOR_RX_CTS,
	NODE_INSPECTOR_RX_DATA,
	NODE_INSPECTOR_RX_ACK,
	NODE_INSPECTOR_TX_RTS,
	NODE_INSPECTOR_TX_CTS,
	NODE_INSPECTOR_TX_DATA,
	NODE_INSPECTOR_TX_ACK,
	NODE_INSPECTOR_DATA_QUEUED,
	NODE_INSPECTOR_DATA_NOT_QUEUED,
	NODE_INSPECTOR_NETWORK_TICK,
	NODE_INSPECTOR_NETWORK_FORWARDING
};

#endif /* GLOBALS_H_ */
