/* matrix4x3_inverse_transform_plane @0x837064D0 — transform a plane by the matrix inverse: the distance is shifted
 * by the translation's projection onto the normal and divided by the scale (0 if the matrix is degenerate), and the
 * normal is inverse-transformed as a vector. */

#include "headers/real_matrix4x3.h"
#include "headers/real_plane3d.h"
#include "headers/real_vector3d.h"

extern real_vector3d *matrix4x3_inverse_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);

real_plane3d * matrix4x3_inverse_transform_plane(const real_matrix4x3 *matrix, const real_plane3d *plane, real_plane3d *result)
{
    if ( matrix->scale == 0.0f )
    {
        result->d = 0.0f;
    }
    else
    {
        float distance = plane->d
            - ((matrix->n[3][0] * plane->n.n[0])
             + ((matrix->n[3][2] * plane->n.n[2]) + (matrix->n[3][1] * plane->n.n[1])));
        result->d = distance;
        if ( matrix->scale != 1.0f )
            result->d = distance / matrix->scale;
    }
    matrix4x3_inverse_transform_vector(matrix, &plane->n, &result->n);
    return result;
}
