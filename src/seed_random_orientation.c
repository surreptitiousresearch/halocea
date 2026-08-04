/* seed_random_orientation @0x837230F0 — advance `*seed` through 3 iterations of the standard Numerical
 * Recipes LCG (multiplier 1664525, increment 1013904223), use each iteration's high 16 bits (scaled to
 * [0,1]) as three independent random fractions, derive a random direction (theta1/phi spherical angles)
 * for `forward` and its orthogonal `up`, then yaw both by a third independent random angle (theta2) via
 * yaw_vectors.
 *
 * DEVIATION: the decompiler completely garbles this into an `__int64`/HIDWORD/LODWORD pun soup on a
 * misread `__int64 forward` parameter (Hex-Rays flagged "local variable allocation has failed"). DB
 * confirms the real 3-pointer signature (`seed`, `forward`, `up`); rebuilt entirely from disasm
 * register-by-register. The `yaw_vectors` call passes this function's `up`/`forward` in swapped
 * argument positions relative to their names here — confirmed against yaw_vectors' own DB signature
 * (`forward, up, sine, cosine`), not a transcription error. */

#include "headers/real_vector3d.h"
#include "headers/math_constants.h"

extern double sin(double x);
extern double cos(double x);
extern void yaw_vectors(real_vector3d *forward, const real_vector3d *up, float sine, float cosine);

void seed_random_orientation(unsigned int *seed, real_vector3d *forward, real_vector3d *up)
{
    unsigned int state1 = 1664525 * *seed + 1013904223;
    *seed = state1;
    unsigned int state2 = 1664525 * state1 + 1013904223;
    *seed = state2;
    unsigned int state3 = 1664525 * state2 + 1013904223;
    *seed = state3;

    float random_a = (float)(state1 >> 16) * 0.000015259022f;
    float random_b = (float)(state2 >> 16) * 0.000015259022f;
    float random_c = (float)(state3 >> 16) * 0.000015259022f;

    float theta1 = random_a * TWO_PI;
    float phi = random_b * PI - 1.5707964f;
    float theta2 = random_c * TWO_PI;

    float cos_theta1 = (float)cos(theta1);
    float sin_theta1 = (float)sin(theta1);
    float cos_phi = (float)cos(phi);
    float sin_phi = (float)sin(phi);

    forward->n[0] = cos_phi * cos_theta1;
    forward->n[1] = cos_phi * sin_theta1;
    forward->n[2] = sin_phi;

    up->n[0] = -(sin_phi * cos_theta1);
    up->n[1] = -(sin_phi * sin_theta1);
    up->n[2] = cos_phi;

    float cos_theta2 = (float)cos(theta2);
    float sin_theta2 = (float)sin(theta2);

    yaw_vectors(up, forward, sin_theta2, cos_theta2);
}
