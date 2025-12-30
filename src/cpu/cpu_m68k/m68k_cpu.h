/*
 * Mango68k
 * m68k_cpu.h
 *
 * Motorola 68000 CPU core state
 *
 * Based on PearPC structure, rewritten for 68K
 */

#ifndef __M68K_CPU_H__
#define __M68K_CPU_H__

#include "system/types.h"
#include <stdbool.h>

/*
 * Motorola 68000 CPU State
 */
typedef struct M68K_CPU_State {

    /* Data registers D0–D7 */
    uint32 d[8];

    /* Address registers A0–A7 (A7 = SP) */
    uint32 a[8];

    /* Program Counter */
    uint32 pc;

    /* Status Register */
    uint16 sr;

    /* Current opcode (debug / decode) */
    uint16 current_opcode;

    /* Internal flags */
    bool stopped;
    bool exception_pending;

    /* Simple cycle counter (optional for v0.1) */
    uint64 cycles;

} M68K_CPU_State;

/* Global CPU instance */
extern M68K_CPU_State gCPU;

/* CPU control */
bool   m68k_cpu_init(void);
void   m68k_cpu_run(void);
void   m68k_cpu_stop(void);

/* Register helpers */
uint32 m68k_cpu_get_pc(int cpu);
void   m68k_cpu_set_pc(int cpu, uint32 value);

#endif /* __M68K_CPU_H__ */
