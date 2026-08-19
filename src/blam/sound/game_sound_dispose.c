/* game_sound_dispose @ 0x83713798 — drop the looping-sound pool pointer */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
void game_sound_dispose(void)
{
    if ( game_looping_sound_data )
        game_looping_sound_data = 0;
}
