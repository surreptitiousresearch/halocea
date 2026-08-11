/* aim_assist_clear_line_of_sight @0x836C57F0 — tests whether the segment p0->p1 has a clear line of
 * sight to a candidate target. Clear if the ray hits nothing, or if it hits an object that shares the
 * target's ultimate parent (i.e. the target itself). Blocked by world geometry or any other object. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/collision_result_type.h"
#include "headers/collision_test_flags.h"

extern int object_get_ultimate_parent(int object_index);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);

uint8_t aim_assist_clear_line_of_sight(const real_point3d *p0, const real_point3d *p1, int ignore_object_index, int target_object_index)
{
    int ignore_parent = object_get_ultimate_parent(ignore_object_index);

    real_vector3d delta;
    delta.n[0] = p1->n[0] - p0->n[0];
    delta.n[1] = p1->n[1] - p0->n[1];
    delta.n[2] = p1->n[2] - p0->n[2];

    collision_result result;
    if (!collision_test_vector(_collision_test_for_line_of_sight_flags, p0, &delta, ignore_parent, &result))
        return 1;  /* nothing in the way */

    if (result.type != collision_result_object)
        return 0;  /* hit world geometry (media/mesh/structure), not an object */

    /* Hit an object: clear only if it is (part of) the intended target. */
    if (object_get_ultimate_parent(result.object_index) == object_get_ultimate_parent(target_object_index))
        return 1;

    return 0;
}
