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
    __int16 height = bitmap_mipmap_get_height(source_bitmap, source_mipmap_index);

    for ( __int16 y = 0; y < height; y = (__int16)(y + 4) )
    {
        __int16 width = bitmap_mipmap_get_width(source_bitmap, source_mipmap_index);

        for ( __int16 x = 0; x < width; x = (__int16)(x + 4) )
        {
            S3TC_COLOR decoded[16];

            switch ( (unsigned __int16)source_bitmap->format )
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

            __int16 pixel_index = 0;
            for ( __int16 row = 0; row < 4; row = (__int16)(row + 1) )
            {
                for ( __int16 col = 0; col < 4; col = (__int16)(col + 1) )
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
