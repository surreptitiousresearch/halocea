/* rasterizer_xbox_bitmap_swizzle @0x837E3878 — convert a bitmap's pixel data into the Xbox 360 GPU's swizzled
 * (Morton-order tiled) layout in place (source: rasterizer_swizzle.c). Skips bitmaps already flagged 0x2 or 0x10.
 * For every mipmap it allocates a scratch buffer, computes the swizzle masks for that level's dimensions, and
 * swizzles source -> scratch using the routine matching the texture kind (2D, 3D volume, or the six faces of a
 * cube map) and the pixel size (1/2/4 bytes), then copies the swizzled data back over the mipmap and frees the
 * scratch. Each processed bitmap is marked swizzled (flag 0x8).
 *
 * DEVIATION: bytes_per_pixel is `bits_per_pixel / 8` — Hex-Rays shows the signed-division expansion
 * `(bits>>3) + (bits<0 && (bits&7)!=0)`, which is the same for the always-positive bit depths here. The
 * type dispatch (0 = 2D, 1 = 3D, 2 = cube) is reproduced as in the decompiler; unknown byte sizes and texture
 * types simply do nothing. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include <string.h>
#include "headers/bitmap_type.h"
#include "headers/bitmap_flags.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern int bitmap_mipmap_get_pixel_data_size(const bitmap_data *bitmap, int16_t mipmap_index);
extern int16_t bitmap_mipmap_get_width(const bitmap_data *bitmap, int16_t mipmap_index);
extern int16_t bitmap_mipmap_get_height(const bitmap_data *bitmap, int16_t mipmap_index);
extern __int16 bitmap_mipmap_get_depth(const bitmap_data *bitmap, __int16 mipmap_index);
extern int16_t bitmap_format_get_bits_per_pixel(int16_t format);
extern void compute_swizzle_masks(int16_t width, int16_t height, int16_t depth);
extern void rasterizer_xbox_bitmap_swizzle2d_byte(uint8_t *dst, const uint8_t *src, int16_t width, int16_t height);
extern void rasterizer_xbox_bitmap_swizzle2d_word(void *dst, const void *src, int16_t width, int16_t height);
extern void rasterizer_xbox_bitmap_swizzle2d_long(void *dst, const void *src, int16_t width, int16_t height);
extern void rasterizer_xbox_bitmap_swizzle3d_byte(uint8_t *dst, const uint8_t *src, int16_t width, int16_t height, int16_t depth);
extern void rasterizer_xbox_bitmap_swizzle3d_word(char *dst, char *src, int16_t width, int16_t height, int16_t depth);
extern void rasterizer_xbox_bitmap_swizzle3d_long(char *dst, char *src, int16_t width, int16_t height, int16_t depth);

void rasterizer_xbox_bitmap_swizzle(bitmap_data *bitmap)
{
    if ( (bitmap->flags & (1u << _bitmap_compressed_bit)) != 0 || (bitmap->flags & (1u << _bitmap_linear_bit)) != 0 )
        return;
    if ( (unsigned __int16)bitmap->mipmap_count >= 0x8000u )
        return;

    for ( int mipmap = 0; mipmap <= bitmap->mipmap_count; ++mipmap )
    {
        int pixel_data_size = bitmap_mipmap_get_pixel_data_size(bitmap, mipmap);
        char *pixels = bitmap_mipmap_address(bitmap, mipmap);
        char *scratch = dlMalloc(pixel_data_size,
                "D:\\Projects\\code\\HCEX\\sources\\rasterizer\\rasterizer_swizzle.c", 0x15F);
        __int16 width = bitmap_mipmap_get_width(bitmap, mipmap);
        __int16 height = bitmap_mipmap_get_height(bitmap, mipmap);
        __int16 depth = bitmap_mipmap_get_depth(bitmap, mipmap);
        if ( !scratch )
            continue;

        __int16 bits_per_pixel = bitmap_format_get_bits_per_pixel(bitmap->format);
        int bytes_per_pixel = bits_per_pixel / 8;
        compute_swizzle_masks(width, height, depth);

        if ( bitmap->type != bitmap_type_2d )
        {
            if ( bitmap->type == bitmap_type_3d )
            {
                switch ( bytes_per_pixel )
                {
                    case 1: rasterizer_xbox_bitmap_swizzle3d_byte(scratch, pixels, width, height, depth); break;
                    case 2: rasterizer_xbox_bitmap_swizzle3d_word(scratch, pixels, width, height, depth); break;
                    case 4: rasterizer_xbox_bitmap_swizzle3d_long(scratch, pixels, width, height, depth); break;
                }
            }
            else if ( (unsigned __int16)bitmap->type < bitmap_type_white )
            {
                int face_size = pixel_data_size / 6;
                char *face_src = pixels;
                char *face_dst = scratch;
                for ( int face = 0; face < 6; ++face )
                {
                    switch ( bytes_per_pixel )
                    {
                        case 1: rasterizer_xbox_bitmap_swizzle2d_byte(face_dst, face_src, width, height); break;
                        case 2: rasterizer_xbox_bitmap_swizzle2d_word(face_dst, face_src, width, height); break;
                        case 4: rasterizer_xbox_bitmap_swizzle2d_long(face_dst, face_src, width, height); break;
                    }
                    face_dst += face_size;
                    face_src += face_size;
                }
            }
        }
        else
        {
            switch ( bytes_per_pixel )
            {
                case 1: rasterizer_xbox_bitmap_swizzle2d_byte(scratch, pixels, width, height); break;
                case 2: rasterizer_xbox_bitmap_swizzle2d_word(scratch, pixels, width, height); break;
                case 4: rasterizer_xbox_bitmap_swizzle2d_long(scratch, pixels, width, height); break;
            }
        }

        memcpy(pixels, scratch, pixel_data_size);
        dlFree(scratch);
        bitmap->flags |= 1u << _bitmap_swizzled_bit;
    }
}
