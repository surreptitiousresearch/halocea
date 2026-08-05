/* bitmap_2d_uncompress_from_mipmap @0x8377CD88 — decompress one S3TC-encoded mipmap level of
 * `source_bitmap` into `destination_bitmap`'s pixels, block by block (4x4 texels per block), dispatching on
 * the source bitmap's compression format (14 = DXT1/RGB, 15 = DXT5/4-bit alpha, 16 = DXT3/3-bit alpha).
 * Skips writing any texel that falls outside the destination bitmap's bounds (for mipmaps whose dimensions
 * aren't a multiple of 4). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format.h"
#include "headers/s3tc_block.h"

extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern int16_t bitmap_mipmap_get_width(const bitmap_data *bitmap, int16_t mipmap_index);
extern int16_t bitmap_mipmap_get_height(const bitmap_data *bitmap, int16_t mipmap_index);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern void DecodeBlockRGB(S3TCBlockRGB *pblockSrc, S3TC_COLOR *colorDst);
extern void DecodeBlockAlpha4(S3TCBlockAlpha4 *pblockSrc, S3TC_COLOR *colorDst);
extern void DecodeBlockAlpha3(S3TCBlockAlpha3 *pblockSrc, S3TC_COLOR *colorDst);

void bitmap_2d_uncompress_from_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t source_mipmap_index)
{
    unsigned char *block = bitmap_mipmap_address(source_bitmap, source_mipmap_index);
    int16_t height = bitmap_mipmap_get_height(source_bitmap, source_mipmap_index);

    for ( int16_t y = 0; y < height; y = (int16_t)(y + 4) )
    {
        int16_t width = bitmap_mipmap_get_width(source_bitmap, source_mipmap_index);

        for ( int16_t x = 0; x < width; x = (int16_t)(x + 4) )
        {
            S3TC_COLOR decoded[16];

            switch ( (uint16_t)source_bitmap->format )
            {
                case _bitmap_format_dxt1:
                    DecodeBlockRGB((S3TCBlockRGB *)block, decoded);
                    block += 8;
                    break;
                case _bitmap_format_dxt3:
                    DecodeBlockAlpha4((S3TCBlockAlpha4 *)block, decoded);
                    block += 16;
                    break;
                case _bitmap_format_dxt5:
                    DecodeBlockAlpha3((S3TCBlockAlpha3 *)block, decoded);
                    block += 16;
                    break;
            }

            int16_t pixel_index = 0;
            for ( int16_t row = 0; row < 4; row = (int16_t)(row + 1) )
            {
                for ( int16_t col = 0; col < 4; col = (int16_t)(col + 1) )
                {
                    if ( col + x < destination_bitmap->width && row + y < destination_bitmap->height )
                    {
                        S3TC_COLOR *destination_pixel = (S3TC_COLOR *)bitmap_2d_address(destination_bitmap,
                            col + x, row + y, 0);
                        *destination_pixel = decoded[pixel_index];
                    }
                    ++pixel_index;
                }
            }
        }
    }
}
