/* object_get_velocities @ 0x836EC1E0 — an object's translational and angular velocity, taken from its
 * ultimate parent (attached objects inherit the carrier's motion). Either output may be null. */

#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


void object_get_velocities(int object_index, real_vector3d *translational_velocity,
                           real_vector3d *angular_velocity)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int parent;

    for ( parent = object->object.parent_object_index; parent != -1; parent = object->object.parent_object_index )
        object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent)->datum;

    if ( translational_velocity )
        *translational_velocity = object->object.translational_velocity;
    if ( angular_velocity )
        *angular_velocity = object->object.angular_velocity;
}
