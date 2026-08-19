/* object_offset_interpolation @0x836EC348 — accumulate a residual position offset into an object's
 * interpolation block so the visual position eases toward the new origin over the next frames. Only applies
 * when the object's animation interpolation is active (interpolation_frame_count != 0). The interpolation
 * block lives at object + original_node_orientations.offset; its offset accumulator is float[4..6]. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


void object_offset_interpolation(int object_index, const real_vector3d *offset)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    if (object->object.animation.interpolation_frame_count)
    {
        float *interpolation = (float *)((char *)object + object->object.original_node_orientations.offset);
        interpolation[4] += offset->n[0];
        interpolation[5] += offset->n[1];
        interpolation[6] += offset->n[2];
    }
}
