/* game_engine_progress_to_postgame_rasterize_delay @0x8374A724 — enter the postgame-rasterize-delay
 * phase: reset the fade, arm the 5-second game-ending timer, then walk every player and kill its unit
 * (player datum +52) and clear its controller vibration (player datum +2).
 * DEVIATION: the decompiler's _DWORD* return is the null loop terminator from data_iterator_next; the
 * function is effectively void. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/game_engine_globals.h"
#include "headers/blam_data_globals.h"

extern void unit_kill(int unit_index);
extern void vibrate_player_clear(int16_t local_player_index);

void game_engine_progress_to_postgame_rasterize_delay(void)
{
    game_engine_globals.post_game_options_fade = 0.0f;
    game_engine_globals.mode = game_engine_mode_postgame_rasterize_delay;
    game_engine_globals.game_ending_timer = 5.0f;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        int unit_index = player->unit_index;
        if ( unit_index != -1 )
            unit_kill(unit_index);
        unsigned __int16 player_index = (unsigned __int16)player->local_player_index;
        if ( player_index != 0xFFFF )
            vibrate_player_clear(player_index);
    }
}
