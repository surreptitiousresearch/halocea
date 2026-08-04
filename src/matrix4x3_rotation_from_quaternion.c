/* matrix4x3_rotation_from_quaternion @0x83705AF8 — fill the 3x3 rotation part (and scale=1, translation=0) of an
 * affine 4x3 matrix from a quaternion. Uses the standard q->R formula with s = 2/|q|^2 (s=0 for a zero quaternion).
 * Component grouping is preserved from the decompiler to keep the float arithmetic bit-identical. n[3]=w.
 *
 * Deviation: returns the matrix pointer, not void. The callee never writes r3 (arg0 passes through to blr) and
 * callers consume r3 as the matrix after the call (83705EC0 stw r8,0x28(r3); 83705F04 stfs f0,0(r3)) — legal
 * codegen only for a declared pointer return. */

#include "headers/real_matrix4x3.h"
#include "headers/real_quaternion.h"

real_matrix4x3 *matrix4x3_rotation_from_quaternion(real_matrix4x3 *matrix, const real_quaternion *quaternion)
{
    float qx = quaternion->v.n[0];
    float qy = quaternion->v.n[1];
    float qz = quaternion->v.n[2];
    float qw = quaternion->w;

    float magnitude_squared = (qw * qw) + ((qz * qz) + ((qx * qx) + (qy * qy)));
    float s = (magnitude_squared == 0.0f) ? 0.0f : 2.0f / magnitude_squared;

    float ys = qy * s;
    float zs = qz * s;

    matrix->n[3][0] = 0.0f;
    matrix->n[3][1] = 0.0f;
    matrix->n[3][2] = 0.0f;
    matrix->scale = 1.0f;

    matrix->n[0][1] = (qx * ys) - (qw * zs);
    matrix->n[1][0] = (qx * ys) + (qw * zs);
    matrix->n[0][2] = (qx * zs) + (qw * ys);
    matrix->n[1][2] = (qy * zs) - (qw * (qx * s));
    matrix->n[2][0] = (qx * zs) - (qw * ys);
    matrix->n[2][1] = (qy * zs) + (qw * (qx * s));
    matrix->n[0][0] = 1.0f - ((qz * zs) + (qy * ys));
    matrix->n[1][1] = 1.0f - ((qz * zs) + (qx * (qx * s)));
    matrix->n[2][2] = 1.0f - ((qy * ys) + (qx * (qx * s)));
    return matrix;
}
