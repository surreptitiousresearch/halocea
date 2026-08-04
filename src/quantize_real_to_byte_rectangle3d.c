/* quantize_real_to_byte_rectangle3d @0x83700958 — quantize a real 3D bounds rectangle into byte bounds
 * relative to a parent rectangle: per axis, the lower edge quantized rounding down and the upper edge
 * rounding up over the parent's [lower, upper] range. A rectangle whose first bound is the FLT_MAX
 * "empty" sentinel zeroes all six bytes.
 *
 * DEVIATION (bug class 5-adjacent): the decompiler lost the register threading of `parent`/`rectangle`/
 * `result` through the six calls (uninitialized v8..v28); the obvious per-axis pattern is restored. */

#include <stdint.h>
#include "headers/real_rectangle3d.h"
#include "headers/byte_rectangle3d.h"

extern uint8_t quantize_real_to_byte_lower_bound(float min, float max, float value);
extern uint8_t quantize_real_to_byte_upper_bound(float min, float max, float value);

byte_rectangle3d *quantize_real_to_byte_rectangle3d(const real_rectangle3d *parent,
        const real_rectangle3d *rectangle, byte_rectangle3d *result)
{
    if ( rectangle->n[0] == 3.4028235e38f )
    {
        for ( int i = 0; i < 6; i++ )
            result->n[i] = 0;
        return result;
    }

    result->n[0] = quantize_real_to_byte_lower_bound(parent->n[0], parent->n[1], rectangle->n[0]);
    result->n[1] = quantize_real_to_byte_upper_bound(parent->n[0], parent->n[1], rectangle->n[1]);
    result->n[2] = quantize_real_to_byte_lower_bound(parent->n[2], parent->n[3], rectangle->n[2]);
    result->n[3] = quantize_real_to_byte_upper_bound(parent->n[2], parent->n[3], rectangle->n[3]);
    result->n[4] = quantize_real_to_byte_lower_bound(parent->n[4], parent->n[5], rectangle->n[4]);
    result->n[5] = quantize_real_to_byte_upper_bound(parent->n[4], parent->n[5], rectangle->n[5]);
    return result;
}
