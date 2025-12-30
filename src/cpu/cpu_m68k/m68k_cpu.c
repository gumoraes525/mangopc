/*
 * Mango68k
 * m68k_cpu.c
 *
 * Based on PearPC structure, rewritten for Motorola 68000
 */

#include <string.h>
#include <stdio.h>

#include "cpu/cpu.h"
#include "cpu/m68k_cpu.h"
#include "cpu/m68k_decode.h"
#include "debug/debugger.h"

M68K_CPU_State gCPU;
Debugger *gDebugger = NULL;

bool m68k_cpu_init(void)
{
    memset(&gCPU, 0, sizeof(gCPU));

    /* 68000 reset state */
    gCPU.pc = 0x00000000;
    gCPU.sr = 0x2700; /* supervisor, interrupts masked */

    return true;
}

void m68k_cpu_run(void)
{
    gDebugger = new Debugger();

    while (true) {
        uint16 opcode;

        /* fetch */
        opcode = m68k_read_word(gCPU.pc);

        /* decode + execute */
        m68k_exec_opcode(opcode);

        /* PC increment happens in exec (variable length) */
    }
}

void m68k_cpu_stop(void)
{
    /* placeholder */
}

uint32 m68k_cpu_get_pc(int cpu)
{
    return gCPU.pc;
}

void m68k_cpu_set_pc(int cpu, uint32 value)
{
    gCPU.pc = value;
}
