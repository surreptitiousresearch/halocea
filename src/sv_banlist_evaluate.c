/* sv_banlist_evaluate @0x8372E688 — HaloScript builtin wrapper for the server command sv_banlist.
 *
 * DEVIATION (2026-07-31): sv_banlist is a void no-argument console builtin (disasm: no arg regs read
 * before written; tail-calls banlist_print_to_console); the decompiler renders it as taking the
 * evaluator's (function_index, thread_index, initialize) triple, but it is declared and called zero-arg
 * here. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void sv_banlist(void);

void sv_banlist_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    (void)function_index;
    (void)initialize;
    sv_banlist();
    hs_return(thread_index, 0);
}
