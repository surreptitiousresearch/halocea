/* rasterizer_project_billboard @0x838012A8 — project a world-space point (with a world-space radius) into
 * screen space for a camera-facing billboard. Transforms the point into view space, rejects it if behind the
 * near plane, then does the perspective divide to place the billboard center in pixels (x,y) plus its clipped
 * depth (z, clamped to <= 1), and computes the on-screen half-extents of the radius along the two screen axes.
 * Returns 0 when the radius is non-positive or the point is behind the camera, 1 otherwise.
 *
 * DEVIATION: the DB prototype is (point, radius, projected_center, projected_axes). Hex-Rays swapped the two
 * output pointers and invented a phantom `a5`: disasm prologue (0x838012CC `mr r28,r5` / `mr r27,r6`) shows
 * r28 = projected_center (3 floats) and r27 = projected_axes (2 floats), so the decompiler's `projected_axes`
 * stores are really projected_center and its `*a5`/`a5[1]` stores are really projected_axes. A stray
 * `*(_QWORD*)v20.n = ...` dead store to the local transform result is dropped. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/real_matrix4x3.h"
#include "headers/blam_data_globals.h"

extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

uint8_t rasterizer_project_billboard(const real_point3d *point, float radius, real_point3d *projected_center, real_vector2d *projected_axes)
{
    if ( radius <= 0.0f )
        return 0;

    __int16 viewport_extent_x = global_window_parameters.camera.viewport_bounds.n[3]  /* x1 - x0 */
        - global_window_parameters.camera.viewport_bounds.n[1];
    __int16 viewport_extent_y = global_window_parameters.camera.viewport_bounds.n[2]  /* y1 - y0 */
        - global_window_parameters.camera.viewport_bounds.n[0];

    real_point3d view_point;
    matrix4x3_transform_point(&global_window_parameters.frustum.world_to_view, point, &view_point);

    float (*projection)[4] = global_window_parameters.frustum.projection_matrix;

    float radius_x = (projection[0][0] * radius);
    float radius_y = (projection[1][1] * radius);

    float z_clip = (((projection[0][2] * view_point.n[0])
        + ((projection[2][2] * view_point.n[2]) + (projection[1][2] * view_point.n[1])))
        + projection[3][2]);
    if ( z_clip <= 0.0f )
        return 0;

    float w_clip = (((projection[0][3] * view_point.n[0])
        + ((projection[2][3] * view_point.n[2]) + (projection[1][3] * view_point.n[1])))
        + projection[3][3]);
    float inverse_w = ((float)1.0 / w_clip);

    float depth = (inverse_w * z_clip);

    float axis_x_clip = (((projection[0][1] * view_point.n[0])
        + ((projection[2][1] * view_point.n[2]) + (projection[1][1] * view_point.n[1])))
        + projection[3][1]);
    float center_y = ((-(((inverse_w * axis_x_clip) - (float)1.0)
        * (float)viewport_extent_y)) - (float)1.0);

    float axis_y_clip = (((projection[0][0] * view_point.n[0])
        + ((projection[2][0] * view_point.n[2]) + (projection[1][0] * view_point.n[1])))
        + projection[3][0]);
    float center_x = ((((axis_y_clip * inverse_w) + (float)1.0)
        * (float)viewport_extent_x) - (float)1.0) * (float)0.5;

    projected_center->n[0] = center_x;
    projected_center->n[1] = center_y * (float)0.5;
    projected_center->n[2] = depth < 1.0f ? depth : 1.0f;

    projected_axes->n[0] = (((float)viewport_extent_x * inverse_w) * radius_x) * (float)0.5;
    projected_axes->n[1] = (((float)viewport_extent_y * inverse_w) * radius_y) * (float)0.5;
    return 1;
}
