/* cheat_active_camouflage_local_player_evaluate @0x837296D0 — HaloScript builtin
 * (cheat_active_camouflage_local_player) evaluator. Argument block: [0] = local-player index (short).
 * No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void cheat_active_camouflage_local_player(int16_t local_player_index);

void cheat_active_camouflage_local_player_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        cheat_active_camouflage_local_player(*arguments);
        hs_return(thread_index, 0);
    }
}
