/* yaw_vectors @0x836FC598 — rotate the `forward` vector about the `up` axis by the angle whose sine and
 * cosine are given: forward = forward*cosine + (up x forward)*sine. (sine/cosine are float args; the
 * decompiler widened them to double via the FPR-shadow ABI.) */

#include "headers/real_vector3d.h"

void yaw_vectors(real_vector3d *forward, const real_vector3d *up, float sine, float cosine)
{
    float fx = forward->n[0];
    float fy = forward->n[1];
    float fz = forward->n[2];

    /* up x forward */
    float cross_x = up->n[1] * fz - up->n[2] * fy;
    float cross_y = fx * up->n[2] - up->n[0] * fz;
    float cross_z = up->n[0] * fy - fx * up->n[1];

    forward->n[0] = fx * cosine + cross_x * sine;
    forward->n[2] = fz * cosine + cross_z * sine;
    forward->n[1] = fy * cosine + cross_y * sine;
}
