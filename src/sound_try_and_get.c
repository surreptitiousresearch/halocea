#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

uint8_t sound_try_and_get(int sound_index)
{
    return datum_try_and_get(sound_data, sound_index) != 0;
}
