/* rasterizer_text_handle_mode_change @0x83814940 — rebuild the screen-space orthographic projection matrix
 * (local_matrix_projection, 5 rows x 4) from the current window's viewport size, so text vertices given in
 * pixels map to clip space. Width = viewport.x1-x0, height = viewport.y1-y0.
 *
 * DEVIATION: the decompiler rendered the int->float viewport-size conversions as __PAIR64__ magic-constant
 * shuffles and returned the leftover fcfid magic (0x82000000); the function is logically void. Restored to
 * plain float math. */

#include <stdint.h>
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/blam_data_globals.h"


void rasterizer_text_handle_mode_change(void)
{
    rectangle2d *viewport = &global_window_parameters.camera.viewport_bounds;
    float width = (int16_t)(viewport->n[3] - viewport->n[1]);
    float height = (int16_t)(viewport->n[2] - viewport->n[0]);

    local_matrix_projection[0][0] = 2.0f / width;
    local_matrix_projection[0][1] = 0.0f;
    local_matrix_projection[0][2] = 0.0f;
    local_matrix_projection[0][3] = -1.0f - 1.0f / width;

    local_matrix_projection[1][0] = 0.0f;
    local_matrix_projection[1][1] = -2.0f / height;
    local_matrix_projection[1][2] = 0.0f;
    local_matrix_projection[1][3] = 1.0f / height + 1.0f;

    local_matrix_projection[2][0] = 0.0f;
    local_matrix_projection[2][1] = 0.0f;
    local_matrix_projection[2][2] = 0.0f;
    local_matrix_projection[2][3] = 0.5f;

    local_matrix_projection[3][0] = 0.0f;
    local_matrix_projection[3][1] = 0.0f;
    local_matrix_projection[3][2] = 0.0f;
    local_matrix_projection[3][3] = 1.0f;

    local_matrix_projection[4][0] = 0.0f;
    local_matrix_projection[4][1] = 0.0f;
    local_matrix_projection[4][2] = 0.0f;
    local_matrix_projection[4][3] = 1.0f;
}
