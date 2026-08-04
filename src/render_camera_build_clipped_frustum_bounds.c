/* render_camera_build_clipped_frustum_bounds @0x8376BD04 — turn a normalized screen clip rectangle into view-
 * space frustum bounds for the camera. The clip edges are scaled by 1/tan(vfov/2), with the horizontal edges
 * additionally scaled by the viewport aspect (height/width). Returns 1 with the clipped bounds when the clip
 * is valid and non-degenerate; otherwise writes the full [-1,1] bounds and returns 0. */

#include "headers/render_camera.h"
#include "headers/real_rectangle2d.h"
#include "headers/blam_data_globals.h"

extern double tan(double x);

int render_camera_build_clipped_frustum_bounds(const render_camera *camera, const real_rectangle2d *clip,
                                               real_rectangle2d *frustum_bounds)
{
    unsigned char use_full_bounds = 1;

    if (!debug_no_frustum_clip && clip->n[0] < clip->n[1] && clip->n[2] < clip->n[3])
    {
        float half_field_of_view = camera->vertical_field_of_view * 0.5f;
        int height = camera->viewport_bounds.n[2] - camera->viewport_bounds.n[0];
        int width = camera->viewport_bounds.n[3] - camera->viewport_bounds.n[1];
        float aspect = (float)height / (float)width;
        float inverse_tangent = 1.0f / (float)tan(half_field_of_view);

        frustum_bounds->n[0] = clip->n[0] * (inverse_tangent * aspect);
        frustum_bounds->n[1] = clip->n[1] * (inverse_tangent * aspect);
        frustum_bounds->n[2] = inverse_tangent * clip->n[2];
        frustum_bounds->n[3] = clip->n[3] * inverse_tangent;

        use_full_bounds = (frustum_bounds->n[0] >= frustum_bounds->n[1])
                          || (frustum_bounds->n[2] >= frustum_bounds->n[3]);
    }

    if (!use_full_bounds)
        return 1;

    frustum_bounds->n[0] = -1.0f;
    frustum_bounds->n[1] = 1.0f;
    frustum_bounds->n[2] = -1.0f;
    frustum_bounds->n[3] = 1.0f;
    return 0;
}
