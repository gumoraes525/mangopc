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

    /* Address registers A0–A6 */
    uint32 a[7];

    /* Stack pointers */
    uint32 usp;   /* User Stack Pointer */
    uint32 ssp;   /* Supervisor Stack Pointer */

    /* Program Counter */
    uint32 pc;

    /* Status Register (raw) */
    uint16 sr;

    /* Cached condition codes */
    bool flag_x;
    bool flag_n;
    bool flag_z;
    bool flag_v;
    bool flag_c;

    /* Cached supervisor state */
    bool supervisor;

    /* Current opcode (debug / decode) */
    uint16 current_opcode;

    /* Interrupt level (0–7) */
    uint8 interrupt_level;

    /* Internal flags */
    bool stopped;
    bool exception_pending;

    /* Cycle counter */
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

/* Stack helpers */
static inline uint32 *m68k_get_sp(void)
{
    return gCPU.supervisor ? &gCPU.ssp : &gCPU.usp;
}

#endif /* __M68K_CPU_H__ */
