/* render_camera_screen_to_world @0x8376D368 — unprojects a normalized screen-space point back into world
 * space: the camera's own position is the ray origin, and the ray direction is derived from the frustum's
 * projection matrix (screen offsets divided by the matrix's x/y scale terms) transformed by the frustum's
 * view-to-world matrix. */

#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/real_point2d.h"

#include "headers/real_matrix4x3.h"
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);

void render_camera_screen_to_world(const render_camera *camera, const render_frustum *frustum,
    const real_point2d *screen_point, real_point3d *world_point, real_vector3d *world_vector)
{
    *world_point = camera->position;

    real_vector3d view_direction;
    view_direction.n[0] = (screen_point->n[0] - frustum->projection_matrix[2][0]) / frustum->projection_matrix[0][0];
    view_direction.n[1] = -((frustum->projection_matrix[2][1] + screen_point->n[1]) / frustum->projection_matrix[1][1]);
    view_direction.n[2] = -1.0f;

    matrix4x3_transform_vector(&frustum->view_to_world, &view_direction, world_vector);
}
