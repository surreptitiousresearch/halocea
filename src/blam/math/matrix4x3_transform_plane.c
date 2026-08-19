/* matrix4x3_transform_plane @ 0x83706278 — transform a plane by an affine 4x3 matrix: rotate the
 * normal, then move the plane distance by the scaled distance plus the translation projected onto
 * the rotated normal. */

#include "headers/real_plane3d.h"
#include "headers/real_matrix4x3.h"

real_plane3d *matrix4x3_transform_plane(const real_matrix4x3 *matrix, const real_plane3d *plane,
                                        real_plane3d *result)
{
    float x = plane->n.n[0];
    float y = plane->n.n[1];
    float z = plane->n.n[2];

    float nx = matrix->n[0][0] * x + (matrix->n[2][0] * z + matrix->n[1][0] * y);
    float ny = matrix->n[0][1] * x + (matrix->n[2][1] * z + matrix->n[1][1] * y);
    float nz = matrix->n[0][2] * x + (matrix->n[2][2] * z + matrix->n[1][2] * y);

    result->n.n[0] = nx;
    result->n.n[1] = ny;
    result->n.n[2] = nz;
    result->d = matrix->n[3][0] * nx
                     + (matrix->n[3][2] * nz + (plane->d * matrix->scale + matrix->n[3][1] * ny));
    return result;
}
