/* rasterizer_widget_project_billboard @0x83785C58 */
/* rasterizer_widget_project_billboard 0x83785C58 — project a world-space billboard (center + radius)
 * into screen space. Transforms the center point through the view matrix, then applies the projection
 * matrix to compute screen-pixel center coordinates and half-extents.
 * DEVIATION: decompiler assigned outputs to wrong params — disasm stores confirm:
 *   0(r28)=projected_center->n[0], 4(r28)=n[1], 8(r28)=n[2], 0(r27)=projected_axes->n[0],
 *   4(r27)=projected_axes->n[1]; the 5th decompiler param 'a5' (r7) is never written. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"
#include "headers/real_matrix4x3.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/blam_data_globals.h"

extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

uint8_t rasterizer_widget_project_billboard(const real_point3d *point, float radius, real_point3d *projected_center, real_vector2d *projected_axes)
{
    if ( radius <= 0.0f )
        return 0;

    float (*proj)[4] = global_window_parameters.frustum.projection_matrix;

    float vw = (float)(global_window_parameters.camera.viewport_bounds.n[3]
                       - global_window_parameters.camera.viewport_bounds.n[1]);
    float vh = (float)(global_window_parameters.camera.viewport_bounds.n[2]
                       - global_window_parameters.camera.viewport_bounds.n[0]);

    real_point3d view;
    matrix4x3_transform_point(&global_window_parameters.frustum.world_to_view, point, &view);
    float vx = view.n[0];
    float vy = view.n[1];
    float vz = view.n[2];

    float proj_radius_x = proj[0][0] * radius;
    float proj_radius_y = proj[1][1] * radius;

    float clip_z = proj[0][2]*vx + proj[1][2]*vy + proj[2][2]*vz + proj[3][2];
    if ( clip_z <= 0.0f )
        return 0;

    float clip_w   = proj[0][3]*vx + proj[1][3]*vy + proj[2][3]*vz + proj[3][3];
    float inv_w    = 1.0f / clip_w;
    float ndc_z    = clip_z * inv_w;

    float clip_x   = proj[0][0]*vx + proj[1][0]*vy + proj[2][0]*vz + proj[3][0];
    float clip_y   = proj[0][1]*vx + proj[1][1]*vy + proj[2][1]*vz + proj[3][1];

    /* screen pixel centre: NDC→pixels, y flipped */
    projected_center->n[0] = ((clip_x * inv_w + 1.0f) * vw - 1.0f) * 0.5f;
    projected_center->n[1] = (-(clip_y * inv_w - 1.0f) * vh - 1.0f) * 0.5f;
    projected_center->n[2] = ndc_z < 1.0f ? ndc_z : 1.0f;

    /* screen pixel half-extents of billboard */
    projected_axes->n[0] = vw * inv_w * proj_radius_x * 0.5f;
    projected_axes->n[1] = vh * inv_w * proj_radius_y * 0.5f;

    return 1;
}
