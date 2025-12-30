/*
 * Mango68k
 * m68k_cpu.cc
 *
 * Based on PearPC structure, rewritten for Motorola 68000
 */

#include <cstring>
#include <cstdio>

#include "cpu/cpu.h"
#include "cpu/m68k_cpu.h"
#include "cpu/m68k_decode.h"
#include "cpu/m68k_exceptions.h"
#include "debug/debugger.h"
#include "mem/memory.h"   // you WILL need this

M68K_CPU_State gCPU;
Debugger *gDebugger = nullptr;

bool m68k_cpu_init(void)
{
    std::memset(&gCPU, 0, sizeof(gCPU));

    /* 68000 RESET sequence */
    gCPU.a[7] = mem_read_long(0x00000000); /* SSP */
    gCPU.pc   = mem_read_long(0x00000004); /* PC */
    gCPU.sr   = 0x2700;                    /* Supervisor, IPL=7 */

    gCPU.stopped = false;

    return true;
}

void m68k_cpu_run(void)
{
    gDebugger = new Debugger();

    while (!gCPU.stopped) {
        uint16 opcode;

        /* Fetch */
        opcode = m68k_read_word(gCPU.pc);

        /* Decode + Execute */
        m68k_exec_opcode(opcode);

        /* Exceptions handled centrally */
        if (gCPU.exception_pending) {
            m68k_handle_exception();
        }

        /* Debugger hook */
        if (gDebugger && gDebugger->shouldBreak()) {
            gDebugger->enter();
        }
    }
}

void m68k_cpu_stop(void)
{
    gCPU.stopped = true;
}

uint32 m68k_cpu_get_pc(int /* cpu */)
{
    return gCPU.pc;
}

void m68k_cpu_set_pc(int /* cpu */, uint32 value)
{
    gCPU.pc = value;
}
