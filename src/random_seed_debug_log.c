/* random_seed_debug_log @0x83722E28 — empty in release (debug-only logging compiled out).
 * Param proven by callers: li r3,1 (game_load) / li r3,0 (game_unload) before the call. */
#include <stdint.h>

void random_seed_debug_log(uint8_t enable)
{
}
