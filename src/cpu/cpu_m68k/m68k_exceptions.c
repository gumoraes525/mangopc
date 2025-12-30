/*
 * Mango68k
 * m68k_exceptions.c
 *
 * Motorola 68000 exception handling
 */

#include <stdio.h>

#include "cpu/m68k_cpu.h"
#include "cpu/m68k_exceptions.h"
#include "debug/debug.h"

/*
 * Push a word onto supervisor stack
 */
static inline void push_word(uint16 value)
{
    gCPU.a[7] -= 2;
    m68k_write_word(gCPU.a[7], value);
}

/*
 * Push a long onto supervisor stack
 */
static inline void push_long(uint32 value)
{
    gCPU.a[7] -= 4;
    m68k_write_long(gCPU.a[7], value);
}

/*
 * Core exception handler
 */
void m68k_raise_exception(M68K_Exception exc)
{
    uint32 vector_address;

    /* Enter supervisor mode */
    gCPU.sr |= 0x2000;

    /* Push state */
    push_long(gCPU.pc);
    push_word(gCPU.sr);

    /* Vector table starts at 0x00000000 */
    vector_address = exc * 4;

    /* Load new PC */
    gCPU.pc = m68k_read_long(vector_address);

    gCPU.exception_pending = false;
}
