/* AllSame @0x837E44F0 — encode a 4x4 S3TC/DXT1 block whose 16 texels are all the same RGB color. Packs
 * that color into RGB565 for both endpoints (rgb0 == rgb1, the DXT1 "no interpolation" encoding). If the
 * alpha mask isn't fully opaque (wAlpha != 0xFFFF, DXT1 1-bit-alpha mode), any texel whose corresponding
 * bit is clear is marked transparent (2-bit index 3) in pixbm, and the block's endpoint color is instead
 * re-derived from the last opaque texel found (matching the reference encoder's behavior of ignoring
 * transparent texels' color contribution). */

#include <stdint.h>
#include "headers/s3tc_block.h"

void AllSame(S3TC_COLOR *pcolor, S3TCBlockRGB *pblock, uint16_t wAlpha)
{
    S3TC_COLOR color = *pcolor;
    unsigned __int16 rgb565 = (unsigned __int16)(((8 * (((32 * pcolor->rgba[2]) & 0xFF00) | pcolor->rgba[1])) & 0xFFE0)
        | (pcolor->rgba[0] >> 3));

    pblock->pixbm = 0;
    pblock->rgb0 = rgb565;
    pblock->rgb1 = rgb565;

    if ( wAlpha == 0xFFFF )
        return;

    for ( int i = 0; i < 16; ++i )
    {
        if ( (1u << i) & wAlpha )
            color = pcolor[i];
        else
            pblock->pixbm |= (3u << (2 * i));
    }

    rgb565 = (unsigned __int16)(((8 * (((32 * color.rgba[2]) & 0xFF00) | color.rgba[1])) & 0xFFE0)
        | (color.rgba[0] >> 3));
    pblock->rgb0 = rgb565;
    pblock->rgb1 = rgb565;
}
