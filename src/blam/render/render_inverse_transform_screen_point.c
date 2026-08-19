/* render_inverse_transform_screen_point @0x837E76A8 — unproject a normalized screen-space point (x in
 * [0, 640), y in [0, 480), matching the 1/0.0015625=640 and 1/0.0020833334=480 scale constants) back into
 * a world-space ray: `world_pos` is set to the frustum's near-plane center (world_vertices[4], offset by
 * the — always zero — global_zero_vector3d), and `world_vec` is the vector from that origin to the point
 * bilinearly interpolated between the frustum's near-plane corners (world_vertices[0..2]). */

#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"


void render_inverse_transform_screen_point(const real_point2d *screen_pos, real_point3d *world_pos,
        real_vector3d *world_vec)
{
    float screen_x = screen_pos->n[0] * 0.0015625f;
    float screen_y_fraction = screen_pos->n[1] * 0.0020833334f - 1.0f;

    world_pos->n[0] = render.frustum.world_vertices[4].n[0] + global_zero_vector3d->n[0];
    world_pos->n[1] = render.frustum.world_vertices[4].n[1] + global_zero_vector3d->n[1];
    world_pos->n[2] = render.frustum.world_vertices[4].n[2] + global_zero_vector3d->n[2];

    float target_x = (render.frustum.world_vertices[2].n[0] - render.frustum.world_vertices[0].n[0]) * -screen_y_fraction
                    + ((render.frustum.world_vertices[1].n[0] - render.frustum.world_vertices[0].n[0]) * screen_x
                       + render.frustum.world_vertices[0].n[0]);
    float target_y = (render.frustum.world_vertices[2].n[1] - render.frustum.world_vertices[0].n[1]) * -screen_y_fraction
                    + ((render.frustum.world_vertices[1].n[1] - render.frustum.world_vertices[0].n[1]) * screen_x
                       + render.frustum.world_vertices[0].n[1]);
    float target_z = (render.frustum.world_vertices[2].n[2] - render.frustum.world_vertices[0].n[2]) * -screen_y_fraction
                    + ((render.frustum.world_vertices[1].n[2] - render.frustum.world_vertices[0].n[2]) * screen_x
                       + render.frustum.world_vertices[0].n[2]);

    world_vec->n[0] = target_x - world_pos->n[0];
    world_vec->n[1] = target_y - world_pos->n[1];
    world_vec->n[2] = target_z - world_pos->n[2];
}
