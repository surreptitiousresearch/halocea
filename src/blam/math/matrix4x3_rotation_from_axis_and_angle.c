/* matrix4x3_rotation_from_axis_and_angle @0x83705790 — Rodrigues' rotation matrix about a unit axis given the
 * sine and cosine of the angle. Scale=1, zero translation. Component grouping preserved from the decompiler. */

#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"

void matrix4x3_rotation_from_axis_and_angle(real_matrix4x3 *matrix, const real_vector3d *axis, float sine, float cosine)
{
    float xx = axis->n[0] * axis->n[0];
    float yy = axis->n[1] * axis->n[1];
    float zz = axis->n[2] * axis->n[2];
    float xs = axis->n[0] * sine;
    float ys = axis->n[1] * sine;
    float zs = axis->n[2] * sine;
    float one_minus_cos = 1.0f - cosine;
    float xy_omc = (axis->n[1] * axis->n[0]) * one_minus_cos;
    float xz_omc = (axis->n[2] * axis->n[0]) * one_minus_cos;
    float yz_omc = (axis->n[2] * axis->n[1]) * one_minus_cos;

    matrix->scale = 1.0f;
    matrix->n[0][0] = ((1.0f - xx) * cosine) + xx;
    matrix->n[1][1] = ((1.0f - yy) * cosine) + yy;
    matrix->n[2][2] = ((1.0f - zz) * cosine) + zz;
    matrix->n[0][1] = xy_omc + zs;
    matrix->n[1][0] = xy_omc - zs;
    matrix->n[0][2] = xz_omc - ys;
    matrix->n[2][0] = xz_omc + ys;
    matrix->n[1][2] = xs + yz_omc;
    matrix->n[2][1] = yz_omc - xs;
    matrix->n[3][0] = 0.0f;
    matrix->n[3][1] = 0.0f;
    matrix->n[3][2] = 0.0f;
}
