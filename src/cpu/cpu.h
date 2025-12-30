#ifndef __CPU_H__
#define __CPU_H__

#include "system/types.h"

/*
 * CPU lifecycle
 */
bool cpu_init(void);
void cpu_init_config(void);

void cpu_stop(void);

/* Optional / architecture dependent */
void cpu_wakeup(void);

/*
 * CPU execution
 * May only be called from within the CPU thread
 */
void cpu_run(void);

/*
 * Program counter
 */
uint32 cpu_get_pc(void);
void   cpu_set_pc(uint32 newvalue);

/*
 * Register access (debugger / introspection)
 */
uint32 cpu_get_dreg(int index);   /* D0–D7 */
uint32 cpu_get_areg(int index);   /* A0–A7 */
void   cpu_set_dreg(int index, uint32 value);
void   cpu_set_areg(int index, uint32 value);

/*
 * Status register
 */
uint16 cpu_get_sr(void);
void   cpu_set_sr(uint16 value);

/*
 * Interrupts / exceptions
 */
void cpu_raise_interrupt(int level);   /* 1–7 */
void cpu_lower_interrupt(int level);

void cpu_raise_exception(int vector);

#endif /* __CPU_H__ */
