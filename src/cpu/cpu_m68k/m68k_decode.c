/*
 * Mango68k
 * m68k_decode.c
 *
 * 68000 instruction decoder / dispatcher
 */

#include "cpu/m68k_cpu.h"
#include "cpu/m68k_decode.h"
#include "cpu/m68k_exec.h"
#include "debug/debug.h"

void m68k_decode_and_exec(uint16 opcode)
{
    gCPU.current_opcode = opcode;

    /* Debug: single-step trace */
    SINGLESTEP("M68K opcode @%08x: %04x", gCPU.pc, opcode);

    /* Decode primary opcode groups (top 4 bits) */
    switch (opcode & 0xF000) {

        case 0x0000:
            m68k_exec_group_0(opcode);
            break;

        case 0x1000:
            m68k_exec_group_1(opcode);
            break;

        case 0x2000:
            m68k_exec_group_2(opcode);
            break;

        /* ... */

        default:
            m68k_raise_exception(M68K_EXC_ILLEGAL);
            break;
    }
}
