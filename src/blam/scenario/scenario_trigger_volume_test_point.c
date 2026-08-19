/* scenario_trigger_volume_test_point @0x83703D58 — tests whether a world position lies inside a scenario
 * trigger volume. Type 0 is an axis-aligned box (min/max at float offsets 18..23); type 1 is an oriented
 * box: the position is transformed into the volume's local frame (origin + forward/up vectors) and tested
 * against [0, extents]. Any other type is never inside. Returns 1 if inside, 0 otherwise.
 *
 * AS-BUILT NOTE (bounds sense verified against ours — do not re-litigate): the oriented arm requires
 * each local coordinate strictly inside (0, extent). 0x83703DD8/DE4/DF0 `fcmpu` local x/y/z against
 * +0.0 with `ble` -> return 0 (so each must be > 0); 0x83703DFC/E08/E14 `fcmpu` the same three against
 * extents.n[0..2] at volume +0x54/+0x58/+0x5C with `bge` -> return 0 / final `blt` -> 1 (so each must be
 * < its positive extent). There is no [-extent, 0] arm in this binary for any axis, X included; a port
 * that flips the local-X sign is diverging from HCEX, not correcting it. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_trigger_volume.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/blam_data_globals.h"


extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern real_point3d *matrix4x3_inverse_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

int scenario_trigger_volume_test_point(int16_t trigger_volume_index, const real_point3d *position)
{
    scenario_trigger_volume *volume =
        &((scenario_trigger_volume *)global_scenario->trigger_volumes.address)[trigger_volume_index];

    int within_top;   /* whether the point is below the volume's top (final z bound) */
    if (volume->type)
    {
        if (volume->type != 1)
            return 0;

        const scenario_trigger_volume_oriented *box = &volume->bounding_box;
        real_matrix4x3 frame;
        matrix4x3_from_point_and_vectors(&frame, &box->position, &box->forward, &box->up);
        real_point3d local;
        matrix4x3_inverse_transform_point(&frame, position, &local);

        if (local.n[0] <= 0.0f || local.n[1] <= 0.0f || local.n[2] <= 0.0f
            || local.n[0] >= box->extents.n[0] || local.n[1] >= box->extents.n[1])
            return 0;
        within_top = local.n[2] < box->extents.n[2];
    }
    else
    {
        const real_rectangle3d *rect = &volume->world_aligned_bounding_box.rectangle;
        if (position->n[0] <= rect->__s1.x0)
            return 0;
        if (position->n[1] <= rect->__s1.y0)
            return 0;
        if (position->n[2] <= rect->__s1.z0 || position->n[0] >= rect->__s1.x1 || position->n[1] >= rect->__s1.y1)
            return 0;
        within_top = position->n[2] < rect->__s1.z1;
    }

    if (!within_top)
        return 0;
    return 1;
}
