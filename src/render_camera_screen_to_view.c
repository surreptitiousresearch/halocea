/* render_camera_screen_to_view @0x8376D320 — unprojects a screen-space point into a view-space
 * direction vector (z is fixed at -1.0, i.e. the near-plane depth in view space). `camera` is
 * unused in the body; only `frustum`'s projection matrix is read. */

#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/real_point2d.h"
#include "headers/real_vector3d.h"

void render_camera_screen_to_view(const render_camera *camera, const render_frustum *frustum,
        const real_point2d *screen_point, real_vector3d *view_vector)
{
    view_vector->n[0] = (screen_point->n[0] - frustum->projection_matrix[2][0]) / frustum->projection_matrix[0][0];
    view_vector->n[2] = -1.0f;
    view_vector->n[1] = -((frustum->projection_matrix[2][1] + screen_point->n[1]) / frustum->projection_matrix[1][1]);
}
