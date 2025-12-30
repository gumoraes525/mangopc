#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "system/types.h"
#include <stdbool.h>

/*
 * CPU-independent debugger interface
 * Implemented by the active CPU core
 */
void cpu_set_singlestep_v(bool v,
                          const char *file,
                          int line,
                          const char *infoformat, ...);

void cpu_set_singlestep_nonverbose(bool v);

#define SINGLESTEP(info...) \
    cpu_set_singlestep_v(true, __FILE__, __LINE__, info)

#endif /* __DEBUG_H__ */
