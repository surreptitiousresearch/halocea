/* render_frustum_cube_view_fraction @0x8376CEE0 — estimate the fraction of the screen a world-space axis-aligned
 * box (bounds) covers through the frustum's projection. Only boxes fully in front of the camera (both z extents
 * negative) are measured: each of the four screen edges (left/right from the x extents, bottom/top from the y
 * extents) is projected at both z planes and the extreme (nearest to the box's screen silhouette) is taken, then
 * clamped to the NDC range [-1, 1]. The covered area is (right-left) * (top-bottom) * 0.25 (the [-1,1]x[-1,1]
 * viewport has area 4), returned as the fraction, or 0 if the box projects to nothing. A box straddling the
 * camera plane (far z >= 0) returns 1.0 (assumed to fill the view); a box entirely behind returns 0.0.
 *
 * DEVIATION: Hex-Rays returned `*((float *)&v2 + 1)`, a spurious pun on a double stack slot; disasm
 * (0x8376CEF0/0x8376CF0C and the blr) shows the result is simply the single-precision value in f1 (0.0, 1.0, or
 * the computed area). The per-edge min/max selection and the [-1,1] clamps are reproduced from the decompiler. */

#include "headers/render_frustum.h"
#include "headers/real_rectangle3d.h"

float render_frustum_cube_view_fraction(const render_frustum *frustum, const real_rectangle3d *bounds)
{
    float fraction = 0.0f;
    if ( bounds->z0 < 0.0f )
    {
        if ( bounds->z1 < 0.0f )
        {
            float inv_z0 = -(1.0f / bounds->z0);
            float inv_z1 = -(1.0f / bounds->z1);
            float p00 = frustum->projection_matrix[0][0];
            float p11 = frustum->projection_matrix[1][1];
            float p20 = frustum->projection_matrix[2][0];
            float p21 = frustum->projection_matrix[2][1];

            float left_z0 = (bounds->z0 * p20 + bounds->x0 * p00) * inv_z0;
            float left_z1 = (bounds->z1 * p20 + bounds->x0 * p00) * inv_z1;
            float left = left_z0 <= left_z1 ? left_z0 : left_z1;
            float left_clamped = left <= -1.0f ? -1.0f : left;

            float bottom_z0 = (bounds->y0 * p11 + bounds->z0 * p21) * inv_z0;
            float bottom_z1 = (bounds->z1 * p21 + bounds->y0 * p11) * inv_z1;
            float bottom = bottom_z0 <= bottom_z1 ? bottom_z0 : bottom_z1;
            float bottom_clamped = bottom > -1.0f ? bottom : -1.0f;

            float right_z0 = (bounds->x1 * p00 + bounds->z0 * p20) * inv_z0;
            float right_z1 = (bounds->x1 * p00 + bounds->z1 * p20) * inv_z1;
            float right = right_z0 <= right_z1 ? right_z1 : right_z0;
            float right_clamped = right <= 1.0f ? right : 1.0f;

            float top_z0 = (p11 * bounds->y1 + bounds->z0 * p21) * inv_z0;
            float top_z1 = (p11 * bounds->y1 + bounds->z1 * p21) * inv_z1;
            float top = top_z0 <= top_z1 ? top_z1 : top_z0;
            float top_clamped = top <= 1.0f ? top : 1.0f;

            float area = (right_clamped - left_clamped) * (top_clamped - bottom_clamped) * 0.25f;
            if ( area > 0.0f )
                fraction = area;
        }
        else
        {
            fraction = 1.0f;
        }
    }
    return fraction;
}
