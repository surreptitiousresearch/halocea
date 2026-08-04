/* game_sound_initialize_for_new_map @0x837137B8 — revalidate the game looping-sound pool and reset the
 * background music loop and timing, when game sound is present. */

#include "headers/game_sound_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void game_sound_initialize_for_new_map(void)
{
    if ( game_looping_sound_data )
    {
        data_make_valid(game_looping_sound_data);
        game_sound_globals->background_loop_index = -1;
        game_sound_globals->frame_index = 0;
        game_sound_globals->render_time = 0;
    }
}
