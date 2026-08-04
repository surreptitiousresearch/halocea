/* scripting_magic_melee_attack_evaluate @0x83728D08 — HaloScript builtin wrapper.
 *
 * DEVIATION (2026-07-31): scripting_magic_melee_attack is a void no-argument builtin (disasm: no arg regs
 * read before written; tail-calls unit_melee_attack_begin); the decompiler renders it as taking the
 * evaluator (function_index, thread_index, initialize) triple, but it is declared and called zero-arg
 * here. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void scripting_magic_melee_attack(void);

void scripting_magic_melee_attack_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    (void)function_index;
    (void)initialize;
    scripting_magic_melee_attack();
    hs_return(thread_index, 0);
}
