#ifndef __M68K_DECODE_H__
#define __M68K_DECODE_H__

#include "system/types.h"

/* Decode and dispatch a 68000 opcode */
void m68k_decode_and_exec(uint16 opcode);

#endif /* __M68K_DECODE_H__ */
