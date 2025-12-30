/*
 * Mango68k
 * debugger.cc
 *
 * Minimal, CPU-agnostic debugger core
 * Safe for Motorola 68000 bring-up
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "debug/debugger.h"
#include "debug/debug.h"
#include "cpu/cpu.h"
#include "cpu/m68k_cpu.h"

/* Global debugger instance (optional) */
Debugger *gDebugger = nullptr;

/* ------------------------------------------------------------
 * Debugger core
 * ------------------------------------------------------------ */

Debugger::Debugger()
{
    singlestep = false;
    verbose = true;
}

Debugger::~Debugger()
{
}

/* ------------------------------------------------------------
 * Single-step control
 * ------------------------------------------------------------ */

void cpu_set_singlestep_v(bool v,
                          const char *file,
                          int line,
                          const char *infoformat,
                          ...)
{
    if (!gDebugger)
        return;

    gDebugger->singlestep = v;

    if (!infoformat)
        return;

    printf("[SINGLESTEP] %s:%d: ", file, line);

    va_list ap;
    va_start(ap, infoformat);
    vprintf(infoformat, ap);
    va_end(ap);

    printf("\n");
}

void cpu_set_singlestep_nonverbose(bool v)
{
    if (!gDebugger)
        return;

    gDebugger->singlestep = v;
    gDebugger->verbose = false;
}

/* ------------------------------------------------------------
 * CPU control hooks
 * ------------------------------------------------------------ */

void Debugger::onInstruction()
{
    if (!singlestep)
        return;

    dumpRegisters();
    waitForUser();
}

/* ------------------------------------------------------------
 * Register dump (Motorola 68000)
 * ------------------------------------------------------------ */

void Debugger::dumpRegisters()
{
    printf("\n---- M68K CPU STATE ----\n");

    printf("PC = %08X   SR = %04X\n", gCPU.pc, gCPU.sr);

    for (int i = 0; i < 8; i++)
        printf("D%d=%08X ", i, gCPU.d[i]);
    printf("\n");

    for (int i = 0; i < 8; i++)
        printf("A%d=%08X ", i, gCPU.a[i]);
    printf("\n");

    printf("-----------------------\n");
}

/* ------------------------------------------------------------
 * Simple debugger prompt
 * ------------------------------------------------------------ */

void Debugger::waitForUser()
{
    char cmd[32];

    while (true) {
        printf("(dbg) ");
        if (!fgets(cmd, sizeof(cmd), stdin))
            continue;

        if (cmd[0] == 'c') {
            singlestep = false;
            return;
        }

        if (cmd[0] == 's') {
            singlestep = true;
            return;
        }

        if (cmd[0] == 'r') {
            dumpRegisters();
            continue;
        }

        if (cmd[0] == 'q') {
            printf("Debugger requested CPU stop.\n");
            cpu_stop();
            return;
        }

        printf("Commands: s=step  c=continue  r=regs  q=quit\n");
    }
}
