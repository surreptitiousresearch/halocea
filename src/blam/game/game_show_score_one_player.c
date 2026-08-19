/* game_show_score_one_player @0x8374D750 */
#include <stdint.h>

extern void multiplayer_message(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

void game_show_score_one_player(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate)
{
    if ( message != -1 )
        multiplayer_message(recipient_player_index, subject_player_index, message, message_data, should_replicate);
}
