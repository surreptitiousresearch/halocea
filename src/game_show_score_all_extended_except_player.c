/* game_show_score_all_extended_except_player @0x8374D968 — broadcasts a scoring/status message to every
 * in-use player except `excepted_player`. Mirrors the broadcast half of game_show_score_extended.c. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


extern void multiplayer_message(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

void game_show_score_all_extended_except_player(int excepted_player, int subject_player_index, int message,
    int message_data, uint8_t should_replicate)
{
    data_iterator it;
    data_iterator_new(&it, player_data);

    while ( data_iterator_next(&it) )
    {
        if ( it.index != excepted_player && message != -1 )
        {
            int index = (subject_player_index == -1) ? it.index : subject_player_index;
            multiplayer_message(it.index, index, message, message_data, should_replicate);
        }
    }
}
