#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include "DefaultConfiguration.h"

#define ReadBit(var, bit)												(((var) & (1 << (bit))) != 0)
#define SetBit(var, bit)												((var) |= (1 << (bit)))
#define ClearBit(var, bit)											((var) &= ~(1 << (bit)))
#define ToggleBit(var, bit)											((var) ^= (1 << (bit)))

#define Modify(var, mask, value)								(var = ((var & ~mask) | (value & mask)))

#define false																		(0)
#define true																		(1)
typedef uint8_t bool;

#define NULL																		(0)

#define atomic(action)													do {Critical(); action ; NonCritical();} while(0)

#define Critical()															cli()
#define NonCritical()														sei()
#define Nop()																		__asm__ volatile("nop")
#define lengthof(x)															( sizeof(x) / sizeof(x[0]) )
//#define offsetof(type, member)									__builtin_offsetof (type, member)

#define MemoryBarrier()													__asm__ volatile ("" : : : "memory")

typedef void (*blockHandlerCallback)(uint8_t* data, uint8_t length);
typedef void (*notificationHandlerCallback)();

#endif /* GLOBALS_H_ */
