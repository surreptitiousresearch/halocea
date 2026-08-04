/* compress_real_vector3d_to_int32_clamp @0x837E81C8 — pack a unit-ish vector into a 10-11-11 bit signed
 * fixed-point int (z:10 bits @ scale 511.5, y:11 bits @ scale 1023.5, x:11 bits @ scale 1023.5), clamping
 * each component to [-1, 1] first. DEVIATION: the decompiler shows a bogus second `long double a2`
 * parameter — it's a reused stack slot for the floor() scratch value, not a real argument; the DB's real
 * prototype takes just the vector pointer. */

#include "headers/real_vector3d.h"

extern double floor(double x);

unsigned int compress_real_vector3d_to_int32_clamp(const real_vector3d *v)
{
    float x = v->n[0];
    if ( x < -1.0f ) x = -1.0f;
    else if ( x > 1.0f ) x = 1.0f;
    int x_int = (int)floor(x * 1023.5f) & 0x7FF;

    float y = v->n[1];
    if ( y < -1.0f ) y = -1.0f;
    else if ( y > 1.0f ) y = 1.0f;
    int y_int = (int)floor(y * 1023.5f) & 0x7FF;

    float z = v->n[2];
    if ( z < -1.0f ) z = -1.0f;
    else if ( z > 1.0f ) z = 1.0f;
    int z_int = (int)floor(z * 511.5f);

    return ((z_int << 11) | y_int) << 11 | x_int;
}
