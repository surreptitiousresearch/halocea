/* cheat_active_camouflage_evaluate @0x83729698 — HaloScript builtin evaluator. Takes no script arguments; invokes the
 * engine routine and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; cheat_active_camouflage
 * actually takes none, so it is called with its real (void) signature. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern int cheat_active_camouflage(void);

void cheat_active_camouflage_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    cheat_active_camouflage();
    hs_return(thread_index, 0);
}
