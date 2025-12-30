/*
 * Mango68k
 * m68k_decode.c
 */

#include "m68k_cpu.h"
#include "m68k_decode.h"
#include "system/arch/sysendian.h"
#include "debug/debug.h"

void m68k_exec_opc(void)
{
    /* Fetch next opcode (68000 = 16-bit) */
    uint16 opcode = m68k_fetch_word();

    /* Temporary: stop execution so we know decode is wired */
    SINGLESTEP("m68k opcode: %04x", opcode);

    /* Advance PC already handled by fetch */
}
