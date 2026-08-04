/* game_show_score_extended @0x8374D8B8 — routes a scoring/status message to one player, or (when
 * recipient_player_index == -1) broadcasts it to every in-use player via a data_iterator walk. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"


extern void multiplayer_message(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

void game_show_score_extended(int recipient_player_index, int subject_player_index, int message,
                              int message_data, uint8_t should_replicate)
{
    if (recipient_player_index == -1)
    {
        data_iterator it;
        data_iterator_new(&it, player_data);
        if (!data_iterator_next(&it))
            return;

        do
        {
            if (message != -1)
            {
                int index = (subject_player_index == -1) ? it.index : subject_player_index;
                multiplayer_message(it.index, index, message, message_data, should_replicate);
            }
        } while (data_iterator_next(&it));

        return;
    }

    if (message != -1)
        multiplayer_message(recipient_player_index, subject_player_index, message, message_data, should_replicate);
}
