/* bitmap_format_to_a8r8g8b8 @0x836F46A8 — read one texel of the given bitmap pixel format and expand
 * it to a 32-bit A8R8G8B8 color. Handles the engine's stored formats: A8 (0), Y8 (1), AY8 (2), A8Y8
 * (3), R5G6B5 (6), A1R5G5B5 (8), A4R4G4B4 (9), already-32-bit X8R8G8B8/A8R8G8B8 (10/11), and the
 * palettized P8 (17) via global_vector_palette. The bit-replication expressions are reproduced as
 * the decompiler rendered them.
 *
 * DEVIATION: the decompiler returns the uninitialized back_chain slot for the default case; that is
 * an unreachable/garbage path in the original, returned here as 0. */

#include <stdint.h>
#include "headers/bitmap_format.h"
#include "headers/blam_data_globals.h"


/* mipmap_address respelled uint16_t*->void* 2026-07-30: format-dependent texel width, body derives its
 * own byte/short views; bitmap_2d_get_pixel passes a char* base (C4133) */
unsigned int bitmap_format_to_a8r8g8b8(int16_t format, void *mipmap_address, int pixel_index)
{
    unsigned char  *bytes  = (unsigned char *)mipmap_address;
    unsigned short *pixels = (unsigned short *)mipmap_address;

    switch (format)
    {
        case _bitmap_format_a8:
            return ((unsigned int)bytes[pixel_index] >> 8) | ((unsigned int)bytes[pixel_index] << 24);
        case _bitmap_format_y8:
            return (((bytes[pixel_index] << 8) | 0xFFFF0000 | bytes[pixel_index]) << 8)
                   | bytes[pixel_index];
        case _bitmap_format_ay8:
        {
            unsigned int rol = ((unsigned int)bytes[pixel_index] << 8) | ((unsigned int)bytes[pixel_index] >> 24);
            return ((((rol | bytes[pixel_index]) << 8) | bytes[pixel_index]) << 8) | bytes[pixel_index];
        }
        case _bitmap_format_a8y8:
        {
            int luminance = (unsigned char)pixels[pixel_index];
            return ((((pixels[pixel_index] & 0xFF00 | luminance) << 8) | luminance) << 8) | luminance;
        }
        case _bitmap_format_r5g6b5:
            return (8 * (((32 * (pixels[pixel_index] & 0xF8FF)) | pixels[pixel_index])
                         & 0xFFFFE01F
                         | (4 * pixels[pixel_index]) & 0x1F80
                         | 0xFFE00000))
                   | (((pixels[pixel_index] >> 1) & 0xE
                       | pixels[pixel_index] & 0xFFF1u) >> 1)
                     & 0x307;
        case _bitmap_format_a1r5g5b5:
        {
            unsigned int p = pixels[pixel_index];
            return (2 * ((4 * ((2 * ((4 * ((8 * p) & 0x3E000 | p & 0x3E0)) | p & 0x7000)) | p & 0x1F))
                         | p & 0x380))
                   | (p >> 2) & 7
                   | (-16777216 * (p >> 15));
        }
        case _bitmap_format_a4r4g4b4:
        {
            unsigned int p = pixels[pixel_index];
            return (16 * ((16 * ((16 * ((16 * ((16 * p) & 0xFFFF0000
                                                | p & 0xFFFFF000
                                                | (16 * ((p >> 4) & 0xF0 | (p >> 8) & 0xF))
                                                | ((unsigned char)p >> 4)))
                                        | ((unsigned char)p >> 4)))
                                | p & 0xF))
                         | p & 0xF));
        }
        case _bitmap_format_x8r8g8b8:
        case _bitmap_format_a8r8g8b8:
            return *(unsigned int *)&pixels[2 * pixel_index];
        case _bitmap_format_p8_bump:
            return global_vector_palette[bytes[pixel_index]];
        default:
            return 0;
    }
}
