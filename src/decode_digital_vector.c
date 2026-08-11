/* decode_digital_vector @0x8379D170 — decode a packed 4-bit-per-axis "digital" direction (2 bits per axis,
 * y then x, each bit0=negative/bit1=positive) into a unit real_vector3d, normalizing the result. Zero input
 * yields the zero vector. */

#include "headers/real_vector3d.h"

extern float __fsqrts(float x);
extern double __fabs(double x);

void decode_digital_vector(unsigned int digital_form, real_vector3d *out)
{
    out->n[0] = 0.0f;
    out->n[1] = 0.0f;
    out->n[2] = 0.0f;

    if ( !digital_form )
        return;

    float *axis = &out->n[1];
    for ( int i = 0; i < 2; ++i, --axis )
    {
        if ( digital_form & 1 )
            *axis = -1.0f;
        if ( (digital_form >> 1) & 1 )
            *axis = 1.0f;
        digital_form >>= 2;
    }

    float length = __fsqrts(out->n[2] * out->n[2] + (out->n[0] * out->n[0] + out->n[1] * out->n[1]));
    if ( __fabs(length) >= 0.0001 )
    {
        float inv_length = 1.0f / length;
        out->n[0] *= inv_length;
        out->n[1] *= inv_length;
        out->n[2] *= inv_length;
    }
}
