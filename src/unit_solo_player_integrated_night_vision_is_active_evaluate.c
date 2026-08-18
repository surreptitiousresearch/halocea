/* unit_solo_player_integrated_night_vision_is_active_evaluate @0x837290A8 — HaloScript builtin wrapper
 * (no arguments); packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t unit_solo_player_integrated_night_vision_is_active(void);

void unit_solo_player_integrated_night_vision_is_active_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs extracts value >> 24) 2026-08-18 */
    int result = (int)((uint8_t)unit_solo_player_integrated_night_vision_is_active()) << 24;
    hs_return(thread_index, result);
}
