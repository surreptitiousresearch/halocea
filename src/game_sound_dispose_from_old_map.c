/* game_sound_dispose_from_old_map @ 0x83714898 — clear looping sounds, invalidate pool */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void game_sound_clear(void);
extern void data_make_invalid(data_array *data);
void game_sound_dispose_from_old_map(void)
{
    if ( game_looping_sound_data && game_looping_sound_data->valid )
    {
        game_sound_clear();
        data_make_invalid(game_looping_sound_data);
    }
}
