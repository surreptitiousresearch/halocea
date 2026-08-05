/* object_force_inside_bsp @0x836F1EA0 — ensure a freshly placed object's origin lies inside the BSP. Casts a
 * ray from a known-good point toward the object's current origin: if nothing is hit and the object is
 * already in a cluster (location.cluster_index != 0xFFFF) the object is fine (return 1). Otherwise, if the collision produced a
 * valid cluster, the object is pulled back to the impact point/location and its node matrices recomputed
 * (return 1). If no valid location could be found, return 0 (caller deletes the object). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/collision_test_flags.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/collision_result.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);
extern void object_compute_node_matrices(int object_index);

uint8_t object_force_inside_bsp(int object_index, int ignore_object_index, const real_point3d *known_good_point)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    real_vector3d delta;
    delta.n[0] = object->object.position.n[0] - known_good_point->n[0];
    delta.n[1] = object->object.position.n[1] - known_good_point->n[1];
    delta.n[2] = object->object.position.n[2] - known_good_point->n[2];

    collision_result result;
    if ( !collision_test_vector(_collision_test_for_projectiles_flags /* 0x1000E9 */, known_good_point, &delta, ignore_object_index, &result)
      && (uint16_t)object->object.location.cluster_index != 0xFFFF )
    {
        return 1;
    }

    if ( (uint16_t)result.location.cluster_index != 0xFFFF )
    {
        object_translate(object_index, &result.point, &result.location);
        object_compute_node_matrices(object_index);
        return 1;
    }
    return 0;
}
