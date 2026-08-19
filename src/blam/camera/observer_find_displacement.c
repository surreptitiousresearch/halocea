/* observer_find_displacement @0x8370FC38 — compute the delta between two observer_derivative-shaped 14-float
 * states (focus_position/focus_offset/focus_distance/field_of_view as flat scalar differences, plus a
 * rotation delta from the trailing forward/up vector pair via matrix4x3_rotation_from_vectors +
 * vector_from_matrices4x3, mirroring observer_rotational_displacement.c). The inputs are raw float arrays
 * (not observer_derivative itself) laid out the same way: [0..2] focus_position, [3..5] focus_offset,
 * [6] focus_distance, [7] field_of_view, [8..10] forward, [11..13] up.
 *
 * DEVIATION: the decompiler rendered the flat scalar loop as repeatedly writing
 * `displacement->focus_position.n[0]` while advancing all three pointers by 4 bytes per iteration — this is
 * the union's flat float view (see observer_derivative.h) being walked one scalar at a time; restored as a
 * plain per-index loop over the first 8 scalars. */

#include "headers/observer_derivative.h"
#include "headers/real_matrix4x3.h"

extern void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern real_vector3d *vector_from_matrices4x3(const real_matrix4x3 *a, const real_matrix4x3 *b, real_vector3d *rotation);

void observer_find_displacement(const float *position0, const float *position1, observer_derivative *displacement)
{
    for ( int i = 0; i < 8; i++ )
        displacement->n[i] = position1[i] - position0[i];

    real_matrix4x3 matrix0;
    real_matrix4x3 matrix1;
    matrix4x3_rotation_from_vectors(&matrix0, (const real_vector3d *)&position0[8], (const real_vector3d *)&position0[11]);
    matrix4x3_rotation_from_vectors(&matrix1, (const real_vector3d *)&position1[8], (const real_vector3d *)&position1[11]);
    vector_from_matrices4x3(&matrix0, &matrix1, &displacement->rotation);
}
