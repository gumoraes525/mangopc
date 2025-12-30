/*
 * Mango68k
 * m68k_exec.c
 *
 * Motorola 68000 instruction execution
 */

#include <stdio.h>

#include "cpu/m68k_cpu.h"
#include "cpu/m68k_exec.h"
#include "cpu/m68k_exceptions.h"
#include "debug/debug.h"

/*
 * Main execution dispatcher
 * (decode already decided instruction class)
 */
void m68k_exec_opcode(uint16 opcode)
{
    gCPU.current_opcode = opcode;

    /* Example: NOP (0x4E71) */
    if (opcode == 0x4E71) {
        /* NOP: do nothing */
        gCPU.cycles += 4;
        return;
    }

    /* STOP (0x4E72) */
    if (opcode == 0x4E72) {
        gCPU.stopped = true;
        return;
    }

    /* Illegal instruction */
    m68k_raise_exception(M68K_EXC_ILLEGAL_INSTR);
}
