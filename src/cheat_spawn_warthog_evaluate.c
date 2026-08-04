/* cheat_spawn_warthog_evaluate @0x83729628 — HaloScript builtin evaluator. Takes no script arguments; invokes the
 * engine routine and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; cheat_spawn_warthog
 * actually takes none, so it is called with its real (void) signature. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void cheat_spawn_warthog(void);

void cheat_spawn_warthog_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    cheat_spawn_warthog();
    hs_return(thread_index, 0);
}
