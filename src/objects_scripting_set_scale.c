/* objects_scripting_set_scale @0x836EDBF8 — set an object's scale (object data +0xB0), starting a smooth
 * interpolation toward it over a frame count unless the object is a type that does not support scale
 * interpolation (object types 5..11).
 *
 * DEVIATION: the decompiler split the trailing argument into a phantom unused parameter plus the used frame
 * count; reconstructed as a single frame_count parameter. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void object_start_interpolation(int object_index, int16_t frame_count);

void objects_scripting_set_scale(int object_index, float scale, int16_t frame_count)
{
    if ( object_index == -1 )
        return;
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    __int16 object_type = object_data->object.type;
    object_data->object.scale = scale;
    /* object_mask_cannot_interpolate == 0xFE0 covers projectile..sound_scenery (object_type.h) */
    if ( ((1 << object_type) & object_mask_cannot_interpolate) == 0 )
        object_start_interpolation(object_index, frame_count);
}
