/* matrix3x3_from_axis_and_angle @0x83706BB0 — Rodrigues rotation matrix from a (unit) axis and the
 * sine/cosine of the rotation angle. The decompiler's double-writes of the off-diagonal slots are the
 * paired ±sine terms sharing one (1-cos) product; written with the final values only. */

#include "headers/real_matrix3x3.h"
#include "headers/real_vector3d.h"

real_matrix3x3 *matrix3x3_from_axis_and_angle(real_matrix3x3 *matrix, const real_vector3d *axis,
        float sine, float cosine)
{
    float one_minus_cosine = 1.0f - cosine;
    float x_sine = axis->n[0] * sine;
    float y_sine = axis->n[1] * sine;
    float z_sine = axis->n[2] * sine;

    matrix->n[0][0] = (1.0f - axis->n[0] * axis->n[0]) * cosine + axis->n[0] * axis->n[0];
    matrix->n[1][1] = (1.0f - axis->n[1] * axis->n[1]) * cosine + axis->n[1] * axis->n[1];
    matrix->n[2][2] = (1.0f - axis->n[2] * axis->n[2]) * cosine + axis->n[2] * axis->n[2];

    float xy = axis->n[1] * axis->n[0] * one_minus_cosine;
    matrix->n[0][1] = xy + z_sine;
    matrix->n[1][0] = xy - z_sine;

    float xz = axis->n[2] * axis->n[0] * one_minus_cosine;
    matrix->n[0][2] = xz - y_sine;
    matrix->n[2][0] = xz + y_sine;

    float yz = axis->n[2] * axis->n[1] * one_minus_cosine;
    matrix->n[1][2] = yz + x_sine;
    matrix->n[2][1] = yz - x_sine;

    return matrix;
}
