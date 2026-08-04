/* player_ui_activate_all_solo_levels_evaluate @0x8372DD98 — HaloScript builtin wrapper.
 *
 * DEVIATION: player_ui_activate_all_solo_levels is a void no-argument builtin; the decompiler renders it as
 * taking the evaluator's (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void player_ui_activate_all_solo_levels(void);

void player_ui_activate_all_solo_levels_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    player_ui_activate_all_solo_levels();
    hs_return(thread_index, 0);
}
