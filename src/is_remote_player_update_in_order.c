/* is_remote_player_update_in_order @0x8376E228 — client-side gate for an incoming remote-player position
 * update: always accepts the first update (no prior sequence number). Otherwise, a wrapping sequence-number
 * comparison: if the new sequence number looks "behind" the last accepted one (difference in [-8,-4)),
 * that's a genuinely stale duplicate/reorder and is thrown away (logged, path "b"); if it looks "ahead" but
 * by less than 4, treat it as in-order noise and accept without logging; anything else is accepted (path "a"
 * only fires for the small stale window on the other side of the wrap). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/remote_player_position_update_header.h"

extern int game_time_get(void);
extern void player_update_client_log_specific_remote_player(player_datum *remote_player, int log_level, const char *format, ...);

uint8_t is_remote_player_update_in_order(player_datum *player, const remote_player_position_update_header *player_update_header)
{
    int last_sequence_number = player->___u26.client_update_data.___u0.remote_player.last_position_update_sequence_number;
    if ( last_sequence_number == -1 )
        return 1;

    int sequence_number = player_update_header->sequence_number;
    int delta = sequence_number - last_sequence_number;

    if ( sequence_number <= last_sequence_number )
    {
        if ( delta + 8 < 4 )
            return 1;

        player_update_client_log_specific_remote_player(player, 1,
            "[%d]b: Threw away remote player position update [%d] (%d), previous ack [%d] (%d).\n",
            game_time_get(), player_update_header->update_id,
            player->___u26.server_update_data.next_remote_player_update_sequence_number,
            sequence_number, last_sequence_number);
        return 0;
    }

    if ( delta < 4 )
        return 1;

    player_update_client_log_specific_remote_player(player, 1,
        "[%d]a: Threw away remote player position update [%d] (%d), previous ack [%d] (%d).\n",
        game_time_get(), player_update_header->update_id,
        player->___u26.server_update_data.next_remote_player_update_sequence_number,
        sequence_number, last_sequence_number);
    return 0;
}
