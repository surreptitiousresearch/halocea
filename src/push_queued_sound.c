/* push_queued_sound @0x837B6A70 */
#include <stdint.h>
#include "headers/queued_mp_sound.h"
#include "headers/blam_data_globals.h"

extern int game_engine_queued_sound_count; /* number of queued sounds */

void push_queued_sound(int player_index, int index, int length, uint8_t should_replicate)
{
    if (game_engine_queued_sound_count < 5)
    {
        int slot = game_engine_queued_sound_count;

        mp_sound_queue[slot].player_index = player_index;
        mp_sound_queue[slot].index = index;
        mp_sound_queue[slot].ticks_left = length;
        game_engine_queued_sound_count = slot + 1;
        mp_sound_queue[slot].should_replicate = should_replicate;
    }
}
