/* render_frustum_get_projection_bounds @0x8376D170 */
/* render_frustum_get_projection_bounds @0x83? — recover the near-plane projection rectangle (left, right,
 * bottom, top at unit depth) from a frustum's projection matrix. The horizontal extents come from the third
 * column's x shear offset ±1 divided by the x scale; the vertical extents likewise from the y shear ±1 divided
 * by the y scale. Both axes are negated because the projection maps -z forward.
 *
 * DEVIATION: the hardware float conversions in the decompiler output are plain single-precision arithmetic;
 * reproduced directly. */

#include "headers/render_frustum.h"
#include "headers/real_rectangle2d.h"

void render_frustum_get_projection_bounds(const render_frustum *frustum, real_rectangle2d *bounds)
{
    float x_shear = -frustum->projection_matrix[2][0];
    float y_shear = -frustum->projection_matrix[2][1];
    float inverse_x_scale = -1.0f / frustum->projection_matrix[0][0];
    float inverse_y_scale = -1.0f / frustum->projection_matrix[1][1];

    bounds->n[0] = (x_shear - 1.0f) * inverse_x_scale;
    bounds->n[1] = (x_shear + 1.0f) * inverse_x_scale;
    bounds->n[3] = (y_shear + 1.0f) * inverse_y_scale;
    bounds->n[2] = (y_shear - 1.0f) * inverse_y_scale;
}
