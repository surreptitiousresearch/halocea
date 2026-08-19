/* unit_set_emotion @0x836CCE38 — set a unit's facial emotion index (unit data +0x2A8) and start a short
 * interpolation so the change blends in. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern void object_start_interpolation(int object_index, int16_t frame_count);

void unit_set_emotion(int unit_index, int16_t emotion_index)
{
    if ( unit_index == -1 )
        return;
    unit_datum *object_data =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    object_data->unit.animation.emotion_index = emotion_index;
    object_start_interpolation(unit_index, 6);
}
