/* garbage_collect_now_evaluate @0x83728010 — HaloScript builtin evaluator. Takes no arguments; invokes the engine routine
 * and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; garbage_collect_now actually
 * takes none, so it is called with its real signature. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void garbage_collect_now(void);

void garbage_collect_now_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    garbage_collect_now();
    hs_return(thread_index, 0);
}
