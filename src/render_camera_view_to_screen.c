/* render_camera_view_to_screen @0x8376D1D8 */
/* render_camera_view_to_screen 0x8376D1D8 — project a view-space point to screen pixels. Returns 0 when the
 * point is behind the camera (view z >= 0) or its normalized device coordinates fall outside [-1, 1]; on success
 * writes the pixel position into screen_point and returns 1. The intermediate NDC is also written to screen_point
 * before the on-screen test (matching the original). */

#include <stdint.h>
#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/real_point3d.h"
#include "headers/real_point2d.h"
#include "headers/rectangle2d.h"

uint8_t render_camera_view_to_screen(const render_camera *camera, const render_frustum *frustum, const real_point3d *view_point, real_point2d *screen_point)
{
    if ( view_point->n[2] >= 0.0f )
        return 0;

    float inverse_depth = (-1.0f / view_point->n[2]);
    float ndc_x = (((frustum->projection_matrix[0][0] * view_point->n[0])
                                  + (frustum->projection_matrix[2][0] * view_point->n[2])) * inverse_depth);
    screen_point->n[0] = ndc_x;
    float ndc_y = -(((frustum->projection_matrix[1][1] * view_point->n[1])
                                   + (frustum->projection_matrix[2][1] * view_point->n[2])) * inverse_depth);
    screen_point->n[1] = ndc_y;

    if ( ndc_x >= -1.0f && ndc_x <= 1.0f && ndc_y >= -1.0f && ndc_y <= 1.0f )
    {
        const rectangle2d *viewport = &camera->viewport_bounds;
        screen_point->n[0] = ((float)(viewport->n[3] - viewport->n[1])
                                     * ((ndc_x + 1.0f) * 0.5f)) + (float)viewport->n[1];
        screen_point->n[1] = (((ndc_y + 1.0f) * 0.5f)
                                     * (float)(viewport->n[2] - viewport->n[0])) + (float)viewport->n[0];
        return 1;
    }
    return 0;
}
