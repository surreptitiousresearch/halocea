#include <stdint.h>
/* bitmap_swizzle_vector3d @0x837E3268 — build the three per-axis Morton-interleave bit masks for a 3D
 * texture of the given dimensions: bit positions are handed out round-robin (x, then y, then z) as long as
 * each axis still has address bits left (its dimension exceeds the current power of two), consuming one
 * input coordinate bit per handed-out position. Used to compose swizzled 3D texel addresses. */

void bitmap_swizzle_vector3d(int16_t dim_x, int16_t dim_y, int16_t dim_z, int16_t x, int16_t y, int16_t z, int *result)
{
    int swizzled_x = 0;
    int swizzled_y = 0;
    int swizzled_z = 0;
    __int16 bit_position = 0;
    __int16 previous_bit_position;

    for ( __int16 axis_bit = 1; ; axis_bit *= 2 )
    {
        previous_bit_position = bit_position;
        if ( axis_bit < dim_x )
        {
            swizzled_x |= (x & 1) << bit_position;
            x >>= 1;
            ++bit_position;
        }
        if ( axis_bit < dim_y )
        {
            swizzled_y |= (y & 1) << bit_position;
            y >>= 1;
            ++bit_position;
        }
        if ( axis_bit < dim_z )
        {
            swizzled_z |= (z & 1) << bit_position;
            z >>= 1;
            ++bit_position;
        }
        if ( previous_bit_position == bit_position )
            break;
    }

    result[0] = swizzled_x;
    result[1] = swizzled_y;
    result[2] = swizzled_z;
}
