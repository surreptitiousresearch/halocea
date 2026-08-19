/* unit_debug_ninja_rope @0x836CA5F8 — debug movement aid. Casts a ray from the unit's camera along
 * its current velocity (scaled x25); if it strikes a near-horizontal surface, snaps the unit onto
 * that point (lifted 0.25 world units). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"

#include "headers/location.h"
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);

void unit_debug_ninja_rope(int unit_index)
{
    real_vector3d   ray;
    real_point3d    camera;
    collision_result result;

    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    unit_get_camera_position(unit_index, &camera);

    /* unit aiming vector (unit+572) scaled to a long probe ray. */
    ray.n[0] = unit->unit.aiming_vector.n[0] * 25.0f;
    ray.n[1] = unit->unit.aiming_vector.n[1] * 25.0f;
    ray.n[2] = unit->unit.aiming_vector.n[2] * 25.0f;

    if (collision_test_vector((1u << _collision_test_back_facing_surfaces_bit) | (1u << _collision_test_structure_bit), &camera, &ray, unit_index, &result))
    {
        if (result.plane.n.n[2] > 0.94999999f)  /* near-horizontal floor */
        {
            result.point.n[2] = result.point.n[2] + 0.25f;
            object_translate(unit_index, &result.point, 0);
        }
    }
}
