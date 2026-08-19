/* looping_sound_find @0x837165D8 — find the active looping-sound datum matching a loop identifier, or -1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/looping_sound_datum.h"
#include "headers/blam_data_globals.h"


/* extern aligned to the callee's definition (src/data_next_index.c; unattested as of wave 1 —
 * propagation will normalize) */
#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);

int looping_sound_find(int identifier)
{
    int index = data_next_index(looping_sound_data, -1);

    if ( index == -1 )
        return -1;

    while ( DATA_ARRAY_ELEMENT(looping_sound_data, looping_sound_datum, index)->loop_identifier != identifier )
    {
        index = data_next_index(looping_sound_data, index);
        if ( index == -1 )
            return -1;
    }
    return index;
}
