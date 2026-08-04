/* is_local_player_update_in_order @0x8376DC4C — sequence-number gate for local player acks: always in
 * order if there's no prior ack (-1), or if the new sequence number is within a small forward/backward
 * window (16) of the last one; anything further out is logged and thrown away. */

#include <stdint.h>
#include "headers/client_log_subject.h"
extern int game_time_get(void);
extern void player_update_client_log(client_log_subject subject, int log_level, const char *format, ...);

uint8_t is_local_player_update_in_order(int sequence_number, int last_update_sequence_number)
{
    if ( last_update_sequence_number == -1 )
        return 1;

    int delta = sequence_number - last_update_sequence_number;

    if ( sequence_number <= last_update_sequence_number )
    {
        if ( delta + 32 >= 16 )
        {
            player_update_client_log(0 /* _client_log_local_player */, 1,
                "[%d]b: Threw away local player ack [%d] (%d).\n",
                game_time_get(), sequence_number, last_update_sequence_number);
            return 0;
        }
        return 1;
    }

    if ( delta < 16 )
        return 1;

    player_update_client_log(0 /* _client_log_local_player */, 1,
        "[%d]a: Threw away local player ack [%d] (%d).\n",
        game_time_get(), sequence_number, last_update_sequence_number);
    return 0;
}
