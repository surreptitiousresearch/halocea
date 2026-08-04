/* game_show_score_you_ally_enemy @0x8374D7E0 — broadcast a scored-event HUD message to every player,
 * picking the message id by their relation to the scoring player: the scorer gets message_you, players on
 * an enemy team get message_enemy, everyone else message_ally (a message id of NONE suppresses that
 * class). The message carries message_data (typically the scoring player) and the replicate flag. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern void multiplayer_message(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

void game_show_score_you_ally_enemy(int player_index, int message_you, int message_ally, int message_enemy, int message_data, uint8_t should_replicate)
{
    player_datum *scoring_player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        int message;
        if ( iterator.index == player_index )
        {
            if ( message_you == -1 )
                continue;
            message = message_you;
        }
        else if ( game_team_is_enemy(scoring_player->team_index, player->team_index) )
        {
            if ( message_enemy == -1 )
                continue;
            message = message_enemy;
        }
        else
        {
            if ( message_ally == -1 )
                continue;
            message = message_ally;
        }
        multiplayer_message(iterator.index, iterator.index, message, message_data, should_replicate);
    }
}
