#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void sound_delete(int sound_index)
{
    datum_delete(sound_data, sound_index);
}
