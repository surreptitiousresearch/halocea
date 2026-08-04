/* display_error_deferred @0x83731020 — queue an error to be shown once the UI is ready, for the given local
 * player (player -1 maps to slot 0). Only the first pending error per slot is kept. */

#include <stdint.h>
#include "headers/widget_globals.h"

void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time)
{
    int slot = local_player_index == -1 ? 0 : local_player_index;
    if ( (unsigned __int16)widget_globals.deferred_error[slot].error_code == 0xFFFF )
    {
        widget_globals.deferred_error[slot].error_code = error_code;
        widget_globals.deferred_error[slot].local_player_index = local_player_index;
        widget_globals.deferred_error[slot].modal = modal;
        widget_globals.deferred_error[slot].pause_game_time = pause_game_time;
    }
}
