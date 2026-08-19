/* cheat_all_powerups_evaluate @0x837295F0 — HaloScript builtin evaluator. Takes no script arguments; invokes the
 * engine routine and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; cheat_all_powerups
 * actually takes none, so it is called with its real (void) signature. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void cheat_all_powerups(void);

void cheat_all_powerups_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    cheat_all_powerups();
    hs_return(thread_index, 0);
}
