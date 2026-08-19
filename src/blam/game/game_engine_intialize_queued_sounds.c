/* game_engine_intialize_queued_sounds @0x837B6CB8 — clear the multiplayer queued-sound ring and prime the
 * first slot (empty, 60-tick lifetime). [Name retains the binary's misspelling of "initialize".] */

#include <string.h>
#include "headers/queued_mp_sound.h"
#include "headers/blam_data_globals.h"

extern int game_engine_queued_sound_count; /* queued-sound dirty/initialized flag (no symbol in DB) */

void game_engine_intialize_queued_sounds(void)
{
    memset(mp_sound_queue, 0, sizeof(mp_sound_queue));
    game_engine_queued_sound_count = 1;
    mp_sound_queue[0].player_index = -1;
    mp_sound_queue[0].index = -1;
    mp_sound_queue[0].ticks_left = 60;
    mp_sound_queue[0].should_replicate = 0;
}
