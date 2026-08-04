/* game_engine_update_multiplayer_sound @0x837B6AD4 — tick the multiplayer queued-sound ring: count down
 * the head entry, and when it expires, pop it (shifting the rest down) and play the next queued sound.
 *
 * DEVIATION: the decompiler drops the leading underscore on the real callees; the disassembly calls
 * _blkmov and the 3-argument internal _game_engine_play_multiplayer_sound(player_index, index,
 * should_replicate). */

#include <stdint.h>
#include "headers/queued_mp_sound.h"
#include "headers/blam_data_globals.h"

extern int  game_engine_queued_sound_count;   /* number of queued sounds */
extern void _blkmov(void *dst, const void *src, int size);
extern void _game_engine_play_multiplayer_sound(int player_index, int index, uint8_t should_replicate);

void game_engine_update_multiplayer_sound(void)
{
    int queued_count = game_engine_queued_sound_count;
    if ( queued_count )
    {
        if ( !--mp_sound_queue[0].ticks_left )
        {
            if ( queued_count > 1 )
                _blkmov(mp_sound_queue, &mp_sound_queue[1], 16 * (queued_count - 1));
            game_engine_queued_sound_count = queued_count - 1;
            if ( queued_count != 1 )
                _game_engine_play_multiplayer_sound(mp_sound_queue[0].player_index, mp_sound_queue[0].index, mp_sound_queue[0].should_replicate);
        }
    }
}
