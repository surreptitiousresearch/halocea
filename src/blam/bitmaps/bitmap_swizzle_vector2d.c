/* bitmap_swizzle_vector2d @0x837E3130 — compute the swizzled (Morton/Z-order-ish) x/y components for one
 * texel of a dim_x by dim_y bitmap, via a bit-interleaving lookup table. The interleaved region only
 * covers the smaller power-of-two dimension (min_log2 bits per axis); a wide/tall texture's excess bits
 * along its longer axis are packed on top of that, shifted into the high bits of whichever output word
 * corresponds to the longer axis. For a swizzled region wider than 64 texels per axis (mask > 63) the
 * table lookup is split into two 6-bit chunks (low 6 bits direct, next chunk via a rotated table lookup);
 * otherwise a single table lookup per axis suffices.
 *
 * DEVIATION: swizzle_table is uint16_t[64], not unsigned int[]. Every access in the disassembly is an
 * `lhzx` (16-bit) at byte offset `2*index` (0x837E31B4/31B8/31F0/31F4/31F8/3200), and the symbol spans
 * exactly 128 bytes (0x82128260, next symbol face_mapping_inverse_table @0x821282E0). The earlier
 * `extern const unsigned int swizzle_table[]` made the four direct-index reads 32-bit with a stride of 4,
 * which read the wrong halfword and, for mask up to 63, ran up to 96 bytes past the end of the table into
 * face_mapping_inverse_table. The two `*(const uint16_t *)((const char *)swizzle_table + ...)` casts were
 * the only correct accesses; with the element type fixed they become plain indexing.
 * Table values are zero-extended halfwords, so the `>> 20` / `>> 31` arms of the rotates are always 0 —
 * they are kept because the instructions really are 32-bit rotates (`rotlwi`). */

#include <stdint.h>

extern int16_t floor_log2(unsigned int k);
extern const uint16_t swizzle_table[64];

void bitmap_swizzle_vector2d(int16_t dim_x, int16_t dim_y, int16_t x, int16_t y, int *result)
{
    int16_t log2_x = floor_log2(dim_x);
    int16_t log2_y = floor_log2(dim_y);
    int16_t min_log2 = (log2_x <= log2_y) ? log2_x : log2_y;

    int16_t mask = (int16_t)((1 << min_log2) - 1);

    unsigned int x_component, y_component;

    if ( mask > 63 )
    {
        int16_t shift_amount = mask >> 6;
        unsigned int y_tbl = swizzle_table[(y >> 6) & shift_amount];
        unsigned int x_tbl = swizzle_table[(x >> 6) & shift_amount];
        unsigned int y_hi = (y_tbl << 12) | (y_tbl >> 20);
        /* (2*i) & 0x7E == 2*(i & 0x3F) — the low 6 bits index the 64-entry table directly */
        x_component = ((x_tbl << 12) | (x_tbl >> 20)) | swizzle_table[x & 0x3F];
        y_component = 2 * (y_hi | swizzle_table[y & 0x3F]);
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
