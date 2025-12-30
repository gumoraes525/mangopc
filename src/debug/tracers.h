#ifndef __TRACERS_H__
#define __TRACERS_H__

#include "tools/snprintf.h"

/*
 * Global trace enable switches
 */
#define TRACE_CPU
#define TRACE_MEM
#define TRACE_IO
#define TRACE_EXC
#define TRACE_OPC

/*
 * CPU tracing (M68K)
 */
#ifdef TRACE_CPU
#define CPU_TRACE(msg...) ht_printf("[CPU] " msg)
#else
#define CPU_TRACE(msg...)
#endif

#ifdef TRACE_OPC
#define OPC_TRACE(msg...) ht_printf("[CPU/OPC] " msg)
#else
#define OPC_TRACE(msg...)
#endif

#ifdef TRACE_EXC
#define EXC_TRACE(msg...) ht_printf("[CPU/EXC] " msg)
#else
#define EXC_TRACE(msg...)
#endif

/*
 * Memory & bus
 */
#ifdef TRACE_MEM
#define MEM_TRACE(msg...) ht_printf("[MEM] " msg)
#else
#define MEM_TRACE(msg...)
#endif

/*
 * I/O
 */
#ifdef TRACE_IO
#define IO_TRACE(msg...) ht_printf("[IO] " msg)
#else
#define IO_TRACE(msg...)
#endif

/*
 * Warnings & errors
 */
#define CPU_WARN(msg...) ht_printf("[CPU] <WARN> " msg)
#define CPU_ERR(msg...)  ht_printf("[CPU] <ERR> " msg)

#define MEM_WARN(msg...) ht_printf("[MEM] <WARN> " msg)
#define MEM_ERR(msg...)  ht_printf("[MEM] <ERR> " msg)

#define IO_WARN(msg...)  ht_printf("[IO] <WARN> " msg)
#define IO_ERR(msg...)   ht_printf("[IO] <ERR> " msg)

#endif /* __TRACERS_H__ */
