/* render_debug_trigger_volumes @0x8368CCF0 — debug visualization: when `debug_trigger_volumes` is enabled, walk
 * every scenario trigger volume and build its world-space frame (an oriented volume's point/forward/up frame, or
 * an identity frame translated to an axis-aligned volume's minimum corner), its local extents, and its world-space
 * diagonal; for each of the volume's 6 edge pairs compute the two world-space edge vectors, and finally cast a
 * collision ray from the camera 95% of the way to the volume's center. Trigger-volume record layout (96 bytes:
 * type word @0, forward @48, up @60, position/bounds @72) matches scenario_trigger_volume_test_point.c.
 *
 * Release-stripped debug body: the rasterizer debug-line draws between the edge transforms — and the use of the
 * camera-to-center occlusion test's result — were compiled out of HCEX_Release, leaving only the opaque calls
 * (matrix4x3_transform_vector, collision_test_vector) whose results are discarded. Reproduced faithfully. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_trigger_volume.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/render_globals.h"
#include "headers/collision_result.h"
#include "headers/collision_test_flags.h"
#include <string.h>
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);

void render_debug_trigger_volumes(void)
{
    if ( !debug_trigger_volumes )
        return;

    scenario *scn = global_scenario;
    for ( int volume_index = 0; volume_index < scn->trigger_volumes.count;
            volume_index = (int16_t)(volume_index + 1) )
    {
        scenario_trigger_volume *volume =
            &((scenario_trigger_volume *)scn->trigger_volumes.address)[volume_index];

        real_matrix4x3 frame;
        real_vector3d extents;
        real_vector3d world_diagonal;
        if ( volume->type )
        {
            /* oriented volume: local frame from point/forward/up, extents @84..92 */
            const scenario_trigger_volume_oriented *box = &volume->bounding_box;
            extents = box->extents;
            world_diagonal = extents;
            matrix4x3_from_point_and_vectors(&frame, &box->position, &box->forward, &box->up);
            matrix4x3_transform_vector(&frame, &extents, &world_diagonal);
        }
        else
        {
            /* axis-aligned volume: identity frame at the minimum corner, extents = max - min */
            const real_rectangle3d *rect = &volume->world_aligned_bounding_box.rectangle;
            memcpy(&frame, global_identity4x3, sizeof(frame));
            frame.___u1.__s1.position.n[0] = rect->__s1.x0;
            frame.___u1.__s1.position.n[1] = rect->__s1.y0;
            frame.___u1.__s1.position.n[2] = rect->__s1.z0;
            extents.n[0] = rect->__s1.x1 - rect->__s1.x0;
            extents.n[1] = rect->__s1.y1 - rect->__s1.y0;
            extents.n[2] = rect->__s1.z1 - rect->__s1.z0;
            world_diagonal = extents;
        }

        /* 6 edge pairs (per axis, +/-): world-space edge vectors along the two other axes.
         * The transformed vectors are unused — the debug edge draws were compiled out. */
        for ( int edge = 0; edge < 6; edge = (int16_t)(edge + 1) )
        {
            real_vector3d edge_b;
            real_vector3d edge_c;
            memset(&edge_b, 0, sizeof(edge_b));
            memset(&edge_c, 0, sizeof(edge_c));
            int axis = (int16_t)(edge / 2);
            int axis_b = (axis + 1) % 3;
            int axis_c = (axis + 2) % 3;
            if ( (uint16_t)(edge % 2) )
            {
                edge_b.n[axis_b] = -extents.n[axis_b];
                edge_c.n[axis_c] = -extents.n[axis_c];
            }
            else
            {
                edge_b.n[axis_b] = extents.n[axis_b];
                edge_c.n[axis_c] = extents.n[axis_c];
            }
            matrix4x3_transform_vector(&frame, &edge_b, &edge_b);
            matrix4x3_transform_vector(&frame, &edge_c, &edge_c);
        }

        /* ray from the camera 95% of the way to the volume's center — result unused (stripped debug) */
        real_vector3d to_center;
        to_center.n[0] = (((world_diagonal.n[0] * 0.5f) + frame.___u1.__s1.position.n[0])
                - render.camera.position.n[0]) * 0.95f;
        to_center.n[1] = (((world_diagonal.n[1] * 0.5f) + frame.___u1.__s1.position.n[1])
                - render.camera.position.n[1]) * 0.95f;
        to_center.n[2] = (((world_diagonal.n[2] * 0.5f) + frame.___u1.__s1.position.n[2])
                - render.camera.position.n[2]) * 0.95f;
        collision_result collision;
        collision_test_vector(_collision_test_for_line_of_sight_flags, &render.camera.position, &to_center, -1, &collision);
    }
}
