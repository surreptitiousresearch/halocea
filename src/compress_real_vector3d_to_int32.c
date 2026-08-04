/* compress_real_vector3d_to_int32 @0x837E8148 — pack a vector's three components into a single 32-bit word:
 * x and y each quantized to 11 bits (scale 1023.5, i.e. assuming components in [-1,1] mapped to [0,2047]),
 * z quantized to 10 bits (scale 511.5), laid out as z:10 | y:11 | x:11 from MSB to LSB.
 *
 * DEVIATION: the decompiler fabricated a trailing "long double a2" parameter — same FPR-shadow phantom
 * documented in map_to_fixed_width.c; the DB's own prototype (and disasm) confirm only 1 real parameter. */

#include <math.h>
#include "headers/real_vector3d.h"

unsigned int compress_real_vector3d_to_int32(const real_vector3d *v)
{
    int x = (int)floor((v->n[0] * 1023.5f));
    int y = (int)floor((v->n[1] * 1023.5f));
    int z = (int)floor((v->n[2] * 511.5f));

    return ((z << 11) | (y & 0x7FF)) << 11 | (x & 0x7FF);
}
