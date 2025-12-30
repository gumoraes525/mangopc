#ifndef __M68K_EXCEPTIONS_H__
#define __M68K_EXCEPTIONS_H__

#include "system/types.h"

/*
 * Motorola 68000 Exception Vectors
 */
typedef enum {
    M68K_EXC_RESET              = 0,
    M68K_EXC_BUS_ERROR          = 2,
    M68K_EXC_ADDRESS_ERROR      = 3,
    M68K_EXC_ILLEGAL_INSTR      = 4,
    M68K_EXC_DIVIDE_BY_ZERO     = 5,
    M68K_EXC_CHK               = 6,
    M68K_EXC_TRAPV              = 7,
    M68K_EXC_PRIVILEGE          = 8,
    M68K_EXC_TRACE              = 9,
    M68K_EXC_LINE_1010          = 10,
    M68K_EXC_LINE_1111          = 11,
    M68K_EXC_SPURIOUS_INTERRUPT = 24,
    M68K_EXC_INTERRUPT_LEVEL_1  = 25
    /* ... up to 31 */
} M68K_Exception;

/* Raise a CPU exception */
void m68k_raise_exception(M68K_Exception exc);

#endif /* __M68K_EXCEPTIONS_H__ */
