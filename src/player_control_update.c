/* player_control_update @ 0x836E0410 — queue this frame's client update and gather input
 * for both local players. (The decompiler exposes a 3rd arg used as the update id passed
 * to update_client_queue_push; the per-player tick arg it forwards is the same value.) */
#include <stdint.h>

extern void update_client_queue_push(int ticks_to_apply_nothingness_to);
extern void handle_one_player_input(int16_t local_player_index, float seconds_elapsed, int ticks_to_apply_action_to);

int player_control_update(float seconds_elapsed, int ticks_to_apply_this_update_to)
{
    int16_t i;
    update_client_queue_push(ticks_to_apply_this_update_to);
    for ( i = 0; i < 2; ++i )
        handle_one_player_input(i, seconds_elapsed, ticks_to_apply_this_update_to);
    return 0;
}
