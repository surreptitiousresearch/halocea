/* cheats_load_evaluate @0x83729718 — HaloScript builtin evaluator. Takes no script arguments; invokes the
 * engine routine and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; cheats_load
 * actually takes none, so it is called with its real (void) signature. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void cheats_load(void);

void cheats_load_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    cheats_load();
    hs_return(thread_index, 0);
}
