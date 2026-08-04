/* observer_rotational_displacement @0x8370F378 — computes the rotational displacement between two
 * orientations (forward0/up0 -> forward1/up1) as a vector, via their matrix representations. */

#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"

extern void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern real_vector3d *vector_from_matrices4x3(const real_matrix4x3 *a, const real_matrix4x3 *b, real_vector3d *rotation);

void observer_rotational_displacement(const real_vector3d *forward0, const real_vector3d *up0,
        const real_vector3d *forward1, const real_vector3d *up1, real_vector3d *rotation)
{
    real_matrix4x3 matrix1;
    real_matrix4x3 matrix0;

    matrix4x3_rotation_from_vectors(&matrix0, forward0, up0);
    matrix4x3_rotation_from_vectors(&matrix1, forward1, up1);
    vector_from_matrices4x3(&matrix0, &matrix1, rotation);
}
