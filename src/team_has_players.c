/* team_has_players @0x8374B8C0 — whether `team_index` still has at least one player who isn't out of the
 * round: with only 0-1 players in the game, always true. Otherwise scans players for the first one who
 * hasn't quit, has a unit (still alive/spawned) OR isn't the multiplayer "odd man out" and, when the
 * variant has a lives limit, hasn't exhausted their lives (deaths >= lives, only checked while they have no
 * unit). If that player's team matches team_index, the team has players. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"


extern int players_in_game(void);
extern uint8_t game_engine_player_is_odd_man_out(int player_index);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

uint8_t team_has_players(int team_index)
{
    if ( players_in_game() <= 1 )
        return 1;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return 0;

    while ( 1 )
    {
        unsigned __int8 qualifies;

        if ( player->quit_out_of_game )
        {
            qualifies = 0;
        }
        else if ( player->unit_index != -1 || !game_engine_player_is_odd_man_out(iterator.index) )
        {
            qualifies = 1;
        }
        else
        {
            unsigned __int8 out_of_lives = 0;
            if ( global_variant.universal_variant.lives > 0 )
            {
                player_datum *player_slot = DATA_ARRAY_ELEMENT(player_data, player_datum, iterator.index);
                if ( player_slot->unit_index == -1 )
                    out_of_lives = player_slot->statistics.deaths >= global_variant.universal_variant.lives;
            }
            qualifies = out_of_lives ? 0 : 1;
        }

        if ( qualifies )
            break;

        player = data_iterator_next(&iterator);
        if ( !player )
            return 0;
    }

    return player->team_index == team_index;
}
