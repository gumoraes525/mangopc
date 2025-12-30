#ifndef __MEM_H__
#define __MEM_H__

#include "system/types.h"
#include <stdbool.h>

/*
 * Physical memory core (CPU-independent)
 */
bool FASTCALL mem_init_physical(uint32 size);
uint32        mem_get_size();

/*
 * DMA-like helpers (implemented by active CPU backend)
 */
bool cpu_dma_write(uint32 dest, const void *src, uint32 size);
bool cpu_dma_read(void *dest, uint32 src, uint32 size);
bool cpu_dma_set(uint32 dest, int c, uint32 size);

/*
 * Optional: framebuffer mapping hook
 * (no-op for CPUs without MMU/BATs)
 */
void cpu_map_framebuffer(uint32 pa, uint32 ea);

#endif /* __MEM_H__ */
