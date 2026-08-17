/* dequantize_byte_to_real_rectangle3d @0x837C44D0 */
/* dequantize_byte_to_real_rectangle3d 0x837C44D0 — expand a byte-quantized 3D rectangle back into world units by
 * interpolating each of the six bounds within the parent rectangle's corresponding axis span (byte/255 of the
 * span, biased by the low edge). A byte of 255 maps exactly to the high edge. The six entries pair up by axis:
 * entries 0/1 use the parent's x span (n[0]..n[1]), 2/3 the y span (n[2]..n[3]), 4/5 the z span (n[4]..n[5]). */

#include "headers/real_rectangle3d.h"
#include "headers/byte_rectangle3d.h"

void dequantize_byte_to_real_rectangle3d(const real_rectangle3d *parent, const byte_rectangle3d *compressed_rectangle, real_rectangle3d *result)
{
    for ( int i = 0; i < 6; ++i )
    {
        int axis_low = 2 * (i / 2);
        unsigned int quantized = compressed_rectangle->n[i];
        if ( quantized == 255 )
            result->n[i] = parent->n[axis_low + 1];
        else
            result->n[i] = ((((float)quantized * 0.0039215689f)
                           * (parent->n[axis_low + 1] - parent->n[axis_low])) + parent->n[axis_low]);
    }
    return;
}
