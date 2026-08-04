/* build_player_speeds @0x8382A6A8 — recompute each player's speed multiplier (word +198, likely a lap/rank
 * counter; float +108, the multiplier itself) based on how far behind the leader they are: finds the
 * maximum counter value across all players, then for each player computes the gap to that maximum (divided
 * by 3 in "race terminator" mode — game_engine_variant.terminator.ignored == 2), giving 1.2x for a gap of
 * 2+, 1.1x for a gap of exactly 1, or 1.0x otherwise (catch-up boost for trailing players). The function's
 * own return value (whatever data_iterator_next last produced, i.e. NULL at loop end) is not meaningful. */

#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/game_variant.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern game_variant *game_engine_get_variant(void);

void build_player_speeds(void)
{
    __int16 max_counter = 0;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        __int16 counter = player->statistics.multiplayer_statistics.race_statistics.laps;
        if ( max_counter <= counter )
            max_counter = counter;
    }

    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        float speed = 1.0f;
        int gap = max_counter - player->statistics.multiplayer_statistics.race_statistics.laps;

        if ( game_engine_get_variant()->game_engine_variant.terminator.ignored == 2 )
            gap /= 3;

        if ( gap < 2 )
        {
            if ( gap >= 1 )
                speed = 1.1f;
        }
        else
        {
            speed = 1.2f;
        }

        player->multiplayer.speed_multiplier = speed;
    }
}
