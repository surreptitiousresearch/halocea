/* render_camera_build_frustum_bounds @0x8376BB28 — derive the normalized projection-plane rectangle for a
 * camera whose pixel viewport sits inside a larger window. Each edge is expressed as the viewport edge's
 * offset from the window centre (2*edge - near_window_edge - far_window_edge), scaled by the inverse window
 * height so the result is aspect-independent; the horizontal edges are additionally multiplied by the
 * viewport's own height/width aspect ratio. The vertical edges are negated so that +y is up.
 *
 * DEVIATION: the decompiler emitted this entirely as __int64/__int128 word shuffles with a failed local
 * allocation; reconstructed from the disassembly. Each integer edge expression is converted int->double->float
 * by hardware fcfid; restored as plain (float)(int) casts. */

#include "headers/render_camera.h"
#include "headers/real_rectangle2d.h"

void render_camera_build_frustum_bounds(const render_camera *camera, real_rectangle2d *frustum_bounds)
{
    int viewport_x0 = camera->viewport_bounds.x0;
    int viewport_x1 = camera->viewport_bounds.x1;
    int viewport_y0 = camera->viewport_bounds.y0;
    int viewport_y1 = camera->viewport_bounds.y1;
    int window_x0 = camera->window_bounds.x0;
    int window_x1 = camera->window_bounds.x1;
    int window_y0 = camera->window_bounds.y0;
    int window_y1 = camera->window_bounds.y1;

    float inverse_window_height = 1.0f / (float)(window_y1 - window_y0);
    float viewport_aspect = (float)(viewport_y1 - viewport_y0) / (float)(viewport_x1 - viewport_x0);

    frustum_bounds->x0 = (float)(2 * viewport_x0 - window_x0 - window_x1) * inverse_window_height * viewport_aspect;
    frustum_bounds->x1 = (float)(2 * viewport_x1 - window_x0 - window_x1) * inverse_window_height * viewport_aspect;
    frustum_bounds->y0 = -((float)(2 * viewport_y1 - window_y0 - window_y1) * inverse_window_height);
    frustum_bounds->y1 = -((float)(2 * viewport_y0 - window_y0 - window_y1) * inverse_window_height);
}
