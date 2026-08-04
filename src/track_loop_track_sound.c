#include <string.h>

#include "headers/data_array.h"
#include "headers/sound_source.h"
#include "headers/looping_sound_datum.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

int track_loop_track_sound(int looping_sound_index, const void *unused, sound_source *source)
{
    looping_sound_datum *looping_sound = datum_try_and_get(looping_sound_data, looping_sound_index);

    if ( !looping_sound )
        return 0;

    memcpy(source, &looping_sound->source, sizeof(sound_source));
    return 1;
}
