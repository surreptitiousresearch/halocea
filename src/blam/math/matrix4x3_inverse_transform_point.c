/* matrix4x3_inverse_transform_point @ 0x83706310 — transform a point by the inverse of an affine
 * 4x3 matrix (uniform scale + rotation + translation): subtract the translation, divide by the
 * scale, then multiply by the rotation's transpose. A zero-scale matrix yields the origin. */

#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"

real_point3d *matrix4x3_inverse_transform_point(const real_matrix4x3 *matrix, const real_point3d *point,
                                                real_point3d *result)
{
    float scale = matrix->scale;

    if ( scale == 0.0f )
    {
        result->n[0] = 0.0f;
        result->n[1] = 0.0f;
        result->n[2] = 0.0f;
        return result;
    }

    {
        float dx = point->n[0] - matrix->n[3][0];
        float dy = point->n[1] - matrix->n[3][1];
        float dz = point->n[2] - matrix->n[3][2];

        if ( scale != 1.0f )
        {
            float inv = 1.0f / scale;
            dx = inv * dx;
            dy = inv * dy;
            dz = inv * dz;
        }

        result->n[0] = matrix->n[0][0] * dx + (matrix->n[0][2] * dz + matrix->n[0][1] * dy);
        result->n[1] = matrix->n[1][0] * dx + (matrix->n[1][2] * dz + matrix->n[1][1] * dy);
        result->n[2] = matrix->n[2][0] * dx + (matrix->n[2][2] * dz + matrix->n[2][1] * dy);
    }
    return result;
}
