/* game_looping_sound_delete @ 0x83713A70 — free a looping-sound datum from the global looping-sound
 * data array. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void game_looping_sound_delete(int looping_sound_index)
{
    datum_delete(game_looping_sound_data, looping_sound_index);
}
