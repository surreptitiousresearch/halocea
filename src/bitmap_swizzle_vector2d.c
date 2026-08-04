/* bitmap_swizzle_vector2d @0x837E3130 — compute the swizzled (Morton/Z-order-ish) x/y components for one
 * texel of a dim_x by dim_y bitmap, via a bit-interleaving lookup table. The interleaved region only
 * covers the smaller power-of-two dimension (min_log2 bits per axis); a wide/tall texture's excess bits
 * along its longer axis are packed on top of that, shifted into the high bits of whichever output word
 * corresponds to the longer axis. For a swizzled region wider than 64 texels per axis (mask > 63) the
 * table lookup is split into two 6-bit chunks (low 6 bits direct, next chunk via a rotated table lookup);
 * otherwise a single table lookup per axis suffices. */

#include <stdint.h>

extern int16_t floor_log2(unsigned int k);
extern const unsigned int swizzle_table[];

void bitmap_swizzle_vector2d(int16_t dim_x, int16_t dim_y, int16_t x, int16_t y, int *result)
{
    __int16 log2_x = floor_log2(dim_x);
    __int16 log2_y = floor_log2(dim_y);
    __int16 min_log2 = (log2_x <= log2_y) ? log2_x : log2_y;

    __int16 mask = (__int16)((1 << min_log2) - 1);

    unsigned int x_component, y_component;

    if ( mask > 63 )
    {
        __int16 shift_amount = mask >> 6;
        unsigned int y_tbl = swizzle_table[(y >> 6) & shift_amount];
        unsigned int x_tbl = swizzle_table[(x >> 6) & shift_amount];
        unsigned int y_hi = (y_tbl << 12) | (y_tbl >> 20);
        x_component = ((x_tbl << 12) | (x_tbl >> 20))
                    | *(const unsigned __int16 *)((const char *)swizzle_table + ((2 * x) & 0x7E));
        y_component = 2 * (y_hi | *(const unsigned __int16 *)((const char *)swizzle_table + ((2 * y) & 0x7E)));
    }
    else
    {
        unsigned int y_lookup = swizzle_table[y & mask];
        x_component = swizzle_table[x & mask];
        y_component = (y_lookup << 1) | (y_lookup >> 31);
    }

    if ( log2_x <= min_log2 )
    {
        result[0] = x_component;
        if ( log2_y > min_log2 )
            y_component |= (y >> min_log2) << (2 * min_log2);
        result[1] = y_component;
    }
    else
    {
        result[1] = y_component;
        result[0] = ((x >> min_log2) << (2 * min_log2)) | x_component;
    }
}
