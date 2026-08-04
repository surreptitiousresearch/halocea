/* game_show_score_team @0x8374D760 — send a HUD `message` to every player on `team` (unless message == -1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void multiplayer_message(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

void game_show_score_team(int team, int message, uint8_t should_replicate)
{
    data_iterator iterator;
    player_datum *player;

    data_iterator_new(&iterator, player_data);
    for ( player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        if ( player->team_index == team && message != -1 )
            multiplayer_message(iterator.index, iterator.index, message, -1, should_replicate);
    }
}
