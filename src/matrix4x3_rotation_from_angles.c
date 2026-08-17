/* matrix4x3_rotation_from_angles @0x83705908 — build an affine rotation matrix (unit scale, zero translation)
 * from yaw/pitch/roll Euler angles.
 *
 * Deviation from decompiler: params attested float (callers lfs single-precision loads from a
 * real_euler_angles3d: yaw@+0->f1, pitch@+4->f2, roll@+8->f3, e.g. 836FD4B0..836FD4C4); the decompiler's
 * double params and its scrambled angle labels ("duplicate cos(pitch)") were FPR-slot artifacts. Disasm
 * trig mapping (83705930..83705974): f28=cos(roll), f29=sin(roll), f27=cos(pitch), f30=sin(pitch),
 * f26=cos(yaw), f12=sin(yaw). The (float) casts model the frsp after each trig call and the fmuls
 * single-precision intermediates. */

#include "headers/real_matrix4x3.h"
#include "headers/fused_math.h"

extern double cos(double);
extern double sin(double);

void matrix4x3_rotation_from_angles(real_matrix4x3 *matrix, float yaw, float pitch, float roll)
{
    float cos_roll  = (float)cos(roll);
    float sin_roll  = (float)sin(roll);
    float cos_pitch = (float)cos(pitch);
    float sin_pitch = (float)sin(pitch);
    float cos_yaw   = (float)cos(yaw);
    float sin_yaw   = (float)sin(yaw);

    matrix->scale = 1.0f;
    matrix->n[3][0] = 0.0f;
    matrix->n[3][1] = 0.0f;
    matrix->n[3][2] = 0.0f;

    matrix->n[0][0] = cos_yaw * cos_pitch;
    matrix->n[2][0] = -sin_pitch;
    matrix->n[2][2] = cos_pitch * cos_roll;
    /* DEVIATION: the four two-term matrix cells are fused — fmadds @0x837059D0/0x837059E0 and
     * fmsubs @0x837059E8/0x837059F0 apply one rounding over a plain fmuls partner term; the
     * sin_pitch*sin_roll / sin_pitch*cos_roll products are the plain fmuls @0x8370597C/0x83705984. */
    matrix->n[1][1] = fused_madd(sin_pitch * sin_roll, sin_yaw, cos_yaw * cos_roll);
    matrix->n[2][1] = -(cos_pitch * sin_roll);
    matrix->n[0][2] = fused_madd(sin_pitch * cos_roll, cos_yaw, sin_yaw * sin_roll);
    matrix->n[0][1] = fused_msub(sin_yaw, cos_roll, (sin_pitch * sin_roll) * cos_yaw);
    matrix->n[1][2] = fused_msub(cos_yaw, sin_roll, (sin_pitch * cos_roll) * sin_yaw);
    matrix->n[1][0] = -(sin_yaw * cos_pitch);
}
