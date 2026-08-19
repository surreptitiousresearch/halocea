/* circle_tangents @0x8382F908 — given a ray direction and its distance from a circle's center (with the
 * circle's radius), computes the two tangent-line directions from the ray's origin to the circle
 * (right_direction/left_direction), plus the along-ray offset to the tangent point (tangent_distance).
 *
 * DEVIATION: the decompiler fabricated two extra trailing float* parameters (`tangent_distance`/`a7`/`a8`
 * in its own numbering); disasm_range(0x8382F908,0x8382F988) confirms the same float-arg-reserves-a-GPR-
 * slot ABI quirk documented for circle_intersect_ray.c — `distance` (f1) reserves r4, `radius` (f2) reserves
 * r5, so right_direction/left_direction/tangent_distance land in r6/r7/r8 exactly matching the DB's 6-param
 * prototype; the decompiler's own "tangent_distance"/"a7"/"a8" locals are actually right_direction,
 * left_direction, and tangent_distance respectively. */

#include <math.h>
#include "headers/real_vector2d.h"

void circle_tangents(const real_vector2d *direction, float distance, float radius,
    real_vector2d *right_direction, real_vector2d *left_direction, float *tangent_distance)
{
    float cos_theta = 1.0f;

    if ( distance != 0.0f )
    {
        cos_theta = radius / distance;
        if ( cos_theta > 1.0f )
            cos_theta = 1.0f;
    }

    float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

    right_direction->n[0] = direction->n[0] * sin_theta + direction->n[1] * cos_theta;
    right_direction->n[1] = -direction->n[0] * cos_theta + direction->n[1] * sin_theta;

    left_direction->n[0] = direction->n[0] * sin_theta - direction->n[1] * cos_theta;
    left_direction->n[1] = direction->n[0] * cos_theta + direction->n[1] * sin_theta;

    *tangent_distance = sin_theta * distance;
}
