/* game_statistics_stop @0x83755190 — finalize per-player statistics at the end of a game: records seconds
 * online (game_time_get()/30), sets games_played=1, and increments games_won for players on the winning
 * team (disasm-confirmed field offsets: statistics.seconds_online @0xBC, .games_played @0x92,
 * .games_won @0x94). Clears the active flag. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/game_statistics_globals.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

void game_statistics_stop(int16_t winning_team_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        player->statistics.seconds_online = game_time_get() / 30;
        player->statistics.games_played = 1;
        if ( player->team_index == winning_team_index )
            ++player->statistics.games_won;
    }

    game_statistics_globals.active = 0;
}
