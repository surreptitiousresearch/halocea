/* pitch_vectors @0x836FC5F0 — rotate a forward/up basis pair about the shared right axis by a precomputed
 * sine/cosine pair, in place. Collapsed the decompiler's v4..v10 temp shuffle back to the 5 real old-value
 * reads it needs (old forward xyz, old up y/z) — algebraically identical, same read-before-write ordering. */

#include "headers/real_vector3d.h"

void pitch_vectors(real_vector3d *forward, real_vector3d *up, float sine, float cosine)
{
    float old_forward_x = forward->n[0];
    float old_forward_y = forward->n[1];
    float old_forward_z = forward->n[2];
    float old_up_y = up->n[1];
    float old_up_z = up->n[2];

    forward->n[0] = up->n[0] * sine + old_forward_x * cosine;
    forward->n[1] = old_forward_y * cosine + sine * old_up_y;
    forward->n[2] = up->n[2] * sine + old_forward_z * cosine;

    up->n[0] = up->n[0] * cosine + -old_forward_x * sine;
    up->n[1] = -old_forward_y * sine + cosine * old_up_y;
    up->n[2] = old_up_z * cosine + -old_forward_z * sine;
}
