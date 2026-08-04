/* object_reset @0x836EBCB0 — zero an object's linear/angular velocity, clear the
 * "at rest" flag bit in its object flags, and reset its type-specific state. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern void object_type_reset(int object_index);

void object_reset(int object_index)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    object_data->object.translational_velocity = *global_zero_vector3d;
    int object_flags = object_data->object.flags;
    object_data->object.angular_velocity = *global_zero_vector3d;
    object_data->object.flags = object_flags & ~(1u << _object_at_rest_bit);

    object_type_reset(object_index);
}
