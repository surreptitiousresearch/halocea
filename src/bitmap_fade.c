/* bitmap_fade @0x8377AC28 — blend every pixel of a bitmap's base mipmap toward `fade_color` by
 * `fade_amount` (clamped to [0,1]; a no-op at or below 0), per byte channel, using 0-256 fixed-point
 * weights with rounding: new = (old*(256-blend) + fade_channel*blend + 127) / 256.
 *
 * DEVIATION: the decompiler renders the per-channel blend as a single packed bit-shift expression over the
 * whole pixel dword (annotated "local variable allocation has failed"); reproduced here as an explicit
 * per-channel loop using value-based shift extraction (matching the decompiler's own
 * HIBYTE/BYTE1/BYTE2/(unsigned __int8) channel macros) rather than a byte-pointer cast, so the result does
 * not depend on host/target endianness assumptions. */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern int bitmap_get_pixel_count(const bitmap_data *bitmap);
extern double floor(double value);

void bitmap_fade(bitmap_data *bitmap, unsigned int fade_color, float fade_amount)
{
    if ( fade_amount <= 0.0f )
        return;
    if ( fade_amount > 1.0f )
        fade_amount = 1.0f;

    int blend = (int)floor(fade_amount * 256.0f + 0.5f);
    int inverse_blend = 256 - blend;

    int weighted_channel[4];
    for ( int c = 0; c < 4; c++ )
        weighted_channel[c] = (int)((fade_color >> (8 * c)) & 0xFF) * blend;

    unsigned int *pixels = bitmap_mipmap_address(bitmap, 0);
    int pixel_count = bitmap_get_pixel_count(bitmap);

    for ( int i = 0; i < pixel_count; i++ )
    {
        unsigned int pixel = pixels[i];
        unsigned int result = 0;
        for ( int c = 0; c < 4; c++ )
        {
            unsigned int channel = (pixel >> (8 * c)) & 0xFF;
            unsigned int blended = (channel * inverse_blend + weighted_channel[c] + 127) >> 8;
            result |= (blended & 0xFF) << (8 * c);
        }
        pixels[i] = result;
    }
}
