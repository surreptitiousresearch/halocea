/* object_set_position @0x836F154C — place an object at a position with an explicit orientation. Disconnects
 * the object from the map's spatial partition, writes position (object data floats [23..25]) and the
 * forward/up basis vectors (forward [29..31], up [32..34]); when no up vector is supplied, derives one
 * orthogonal to forward, then recomputes node matrices and reconnects to the map. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

#include "headers/location.h"
extern void object_disconnect_from_map(int object_index);
extern float normalize3d(real_vector3d *v);
extern void object_compute_node_matrices(int object_index);
extern void object_reconnect_to_map(int object_index, const location *location_in);

void object_set_position(int object_index, const real_point3d *position,
                         const real_vector3d *forward, const real_vector3d *up)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_disconnect_from_map(object_index);

    if ( position )
    {
        object->object.position.n[0] = position->n[0];
        object->object.position.n[1] = position->n[1];
        object->object.position.n[2] = position->n[2];
    }

    if ( forward )
    {
        object->object.forward.n[0] = forward->n[0];
        object->object.forward.n[1] = forward->n[1];
        object->object.forward.n[2] = forward->n[2];

        if ( up )
        {
            object->object.up.n[0] = up->n[0];
            object->object.up.n[1] = up->n[1];
            object->object.up.n[2] = up->n[2];
        }
        else
        {
            /* Derive an up vector orthogonal to forward: start from forward rotated 90 degrees in the
             * XY plane, normalize (falling back to +Z if degenerate), then take its cross product with
             * forward to produce the up basis. */
            float reference_x = forward->n[0];
            real_vector3d horizontal;
            horizontal.n[0] = forward->n[1];
            horizontal.n[1] = -reference_x;
            horizontal.n[2] = 0.0f;

            float ref_y, ref_x, ref_z;
            if ( normalize3d(&horizontal) == 0.0f )
            {
                ref_z = 0.0f;
                ref_x = 1.0f;
                ref_y = 0.0f;
            }
            else
            {
                ref_z = horizontal.n[2];
                ref_y = horizontal.n[1];
                ref_x = horizontal.n[0];
            }

            object->object.up.n[2] = (forward->n[1] * ref_x) - (reference_x * ref_y);
            object->object.up.n[0] = (forward->n[2] * ref_y) - (forward->n[1] * ref_z);
            object->object.up.n[1] = (reference_x * ref_z) - (forward->n[2] * ref_x);
        }
    }

    object_compute_node_matrices(object_index);
    object_reconnect_to_map(object_index, nullptr);
}
