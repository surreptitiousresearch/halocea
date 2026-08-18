/* player_control_action_test_grenade_trigger_evaluate @0x8372B8E0 — HaloScript builtin wrapper (no arguments); packs a boolean result. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern uint8_t player_control_action_test_grenade_trigger(void);

void player_control_action_test_grenade_trigger_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs extracts value >> 24) 2026-08-18 */
    int result = (int)((uint8_t)player_control_action_test_grenade_trigger()) << 24;
    hs_return(thread_index, result);
}
