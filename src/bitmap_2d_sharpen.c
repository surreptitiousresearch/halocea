/* bitmap_2d_sharpen @0x8377B698 — sharpen the top mip of a 2D bitmap in place with a 3x3 unsharp-mask kernel:
 * each output channel byte is positive_table[center] minus negative_table[each of the 8 neighbours], clamped
 * to 0..255. The two 256-entry __int16 lookup tables already bake in the sharpen strength, so the filter is a
 * pure table lookup. Rows wrap vertically and columns wrap horizontally (the leftmost/rightmost pixels sample
 * the opposite edge), so three column passes handle the left edge, the interior, and the right edge. The whole
 * image is filtered into a scratch buffer and copied back so neighbour reads always see the original pixels.
 *
 * DEVIATIONS:
 *  - sharpen_amount is the FINAL argument (float in f1, unused here — the caller has already folded it into
 *    positive_table / negative_table). It was mistakenly placed 2nd in an earlier reconstruction; disasm proves
 *    the ABI order is (bitmap[r3], positive_table[r4], negative_table[r5], sharpen_amount[f1, skips r6 → the
 *    PHANTOM_TAIL]). r4=positive_table = the once-per-pixel CENTER lookup (`lhzx ...,r26` x1); r5=negative_table
 *    = the eight neighbour lookups (`lhzx ...,r31` x8). Reordered 2026-07-31 per disasm 0x8377B6AC/0x8377B6B4.
 *  - The decompiler's `(char *)table + __ROL4__(byte, 1)` is the byte address of table[byte] (rotate-left-1 of
 *    a 0..255 byte == byte*2 == the __int16 index scaled), reconstructed as table[byte].
 *  - The horizontal wrap indices keep the shipped (__int16) truncation of (row_stride +/- x). */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern void *memcpy(void *dst, const void *src, unsigned int size);
extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);

void bitmap_2d_sharpen(bitmap_data *bitmap, const int16_t *positive_table, const int16_t *negative_table, float sharpen_amount)
{
    (void)sharpen_amount;   /* present in the ABI (f1) but unused; folded into the two tables by the caller */
    if ( bitmap->width < 3 || bitmap->height < 3 )
        return;

    unsigned int pixel_data_size = bitmap_get_pixel_data_size(bitmap);
    unsigned char *scratch = dlMalloc(pixel_data_size,
            "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmap_utilities.c", 0x398u);
    if ( !scratch )
        return;

    int height = bitmap->height;
    for ( int y = 0; y < height; y = (__int16)(y + 1) )
    {
        __int16 prev_y = (__int16)(y - 1);
        if ( y <= 0 )
            prev_y = height - 1;
        __int16 next_y = (__int16)(y + 1);
        if ( y >= height - 1 )
            next_y = 0;

        unsigned char *prev_row = (unsigned char *)bitmap_2d_address(bitmap, 0, prev_y, 0);
        unsigned char *cur_row = (unsigned char *)bitmap_2d_address(bitmap, 0, y, 0);
        unsigned char *next_row = (unsigned char *)bitmap_2d_address(bitmap, 0, next_y, 0);
        unsigned char *dest_row = &scratch[4 * bitmap->width * y];

        int row_stride = 4 * bitmap->width;
        int x = 0;

        /* left-edge column: the left neighbour wraps to the opposite end of the row */
        do
        {
            int wrapped_left = (__int16)(row_stride + x);
            int result = positive_table[cur_row[x]]
                    - negative_table[next_row[wrapped_left - 4]]
                    - negative_table[cur_row[wrapped_left - 4]]
                    - negative_table[prev_row[wrapped_left - 4]]
                    - negative_table[prev_row[x + 4]]
                    - negative_table[next_row[x + 4]]
                    - negative_table[cur_row[x + 4]]
                    - negative_table[prev_row[x]]
                    - negative_table[next_row[x]];
            if ( result >= 0 )
                result = result > 255 ? 255 : result;
            else
                result = 0;
            dest_row[x] = (unsigned char)result;
            x = (__int16)(x + 1);
        }
        while ( x < 4 );

        /* interior columns: no wrap */
        for ( int last_interior = (__int16)(4 * bitmap->width - 4); x < last_interior; x = (__int16)(x + 1) )
        {
            int result = positive_table[cur_row[x]]
                    - negative_table[prev_row[x - 4]]
                    - negative_table[prev_row[x + 4]]
                    - negative_table[next_row[x - 4]]
                    - negative_table[next_row[x + 4]]
                    - negative_table[cur_row[x - 4]]
                    - negative_table[cur_row[x + 4]]
                    - negative_table[prev_row[x]]
                    - negative_table[next_row[x]];
            if ( result >= 0 )
                result = result > 255 ? 255 : result;
            else
                result = 0;
            dest_row[x] = (unsigned char)result;
        }

        /* right-edge column: the right neighbour wraps to the start of the row */
        for ( int row_end = (__int16)((__int16)(4 * bitmap->width - 4) + 4); x < row_end; x = (__int16)(x + 1) )
        {
            int wrapped_right = (__int16)(x - 4 * bitmap->width);
            int result = positive_table[cur_row[x]]
                    - negative_table[next_row[wrapped_right + 4]]
                    - negative_table[cur_row[wrapped_right + 4]]
                    - negative_table[prev_row[wrapped_right + 4]]
                    - negative_table[prev_row[x - 4]]
                    - negative_table[next_row[x - 4]]
                    - negative_table[cur_row[x - 4]]
                    - negative_table[prev_row[x]]
                    - negative_table[next_row[x]];
            if ( result >= 0 )
                result = result > 255 ? 255 : result;
            else
                result = 0;
            dest_row[x] = (unsigned char)result;
        }

        height = bitmap->height;
    }

    void *destination = bitmap_mipmap_address(bitmap, 0);
    memcpy(destination, scratch, pixel_data_size);
    dlFree(scratch);
}
