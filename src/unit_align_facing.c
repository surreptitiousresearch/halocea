/* unit_align_facing @0x836CD270 — snap a free (unseated) unit's facing to a 2D alignment vector: the unit's
 * forward becomes (x, y, 0) and its up is reset to world up. No-op for a seated unit. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


void unit_align_facing(int unit_index, const real_vector2d *alignment_vector)
{
    unit_datum *object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( object_data->object.parent_object_index == -1 )
    {
        float x = alignment_vector->n[0];
        object_data->object.forward.n[1] = alignment_vector->n[1];
        object_data->object.forward.n[0] = x;
        object_data->object.forward.n[2] = 0.0f;
        object_data->object.up = *global_up3d;
    }
}
