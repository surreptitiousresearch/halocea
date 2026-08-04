/* uncompress_int32_to_real_vector3d 0x837E8350 — decode a packed 32-bit vector into three floats. The low 11
 * bits, middle 11 bits, and top 10 bits are each treated as a signed field (shifted to the top of a 32-bit word
 * so the sign extends), converted to float, biased by +1, and scaled — the 11-bit fields by 1/2047, the 10-bit
 * field by 1/1023.
 *
 * DEVIATION: the decompiler merged the result pointer (r3) and packed value (r4) into one __int64 and emitted
 * the field shifts as HIDWORD/LODWORD packing; the database prototype (real_vector3d *result, unsigned int i)
 * and the disassembly give the field extraction below. */

#include "headers/real_vector3d.h"

real_vector3d * uncompress_int32_to_real_vector3d(real_vector3d *result, unsigned int i)
{
    int x_field = (int)(i << 21);           /* bits 0..10  -> top of word */
    int y_field = (int)((i >> 11) << 21);   /* bits 11..21 -> top of word */
    int z_field = (int)(i & 0xFFC00000);    /* bits 22..31 already at top */

    result->n[0] = (((float)x_field * 0.00000095367432f) + 1.0f) * 0.00048851978f;
    result->n[1] = (((float)y_field * 0.00000095367432f) + 1.0f) * 0.00048851978f;
    result->n[2] = (((float)z_field * 0.00000047683716f) + 1.0f) * 0.00097751711f;
    return result;
}
