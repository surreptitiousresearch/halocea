/* breakable_surfaces_reset_evaluate @0x837295B8 — HaloScript builtin evaluator. Takes no script arguments; invokes the
 * engine routine and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; breakable_surfaces_reset
 * actually takes none, so it is called with its real (void) signature. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void breakable_surfaces_reset(void);

void breakable_surfaces_reset_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    breakable_surfaces_reset();
    hs_return(thread_index, 0);
}
