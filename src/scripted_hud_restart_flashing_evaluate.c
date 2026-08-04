/* scripted_hud_restart_flashing_evaluate @0x8372CDB0 — HaloScript builtin evaluator. Takes no arguments; invokes the engine routine and
 * returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; scripted_hud_restart_flashing takes none. */
#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void scripted_hud_restart_flashing(void);
void scripted_hud_restart_flashing_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    scripted_hud_restart_flashing();
    hs_return(thread_index, 0);
}
