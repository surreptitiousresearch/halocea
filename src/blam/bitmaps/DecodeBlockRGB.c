/* DecodeBlockRGB @0x837E5018 — decode one 4x4 S3TC/DXT1 RGB block into 16 expanded colors. The two 565
 * endpoint colors are expanded to 8 bits per channel; then, depending on the ordering of the two endpoints,
 * either two interpolated colors (opaque 4-color block, endpoint1 < endpoint0) or one averaged color plus
 * transparent black (1-bit-alpha 3-color block) fill the 4-entry palette. The 2-bit-per-pixel index bitmap
 * then selects a palette entry for each of the 16 output texels. A null source block yields 64 zero bytes.
 *
 * The palette is reproduced as a 16-byte array laid out in the original big-endian memory order: each color
 * occupies 4 bytes {blue, green, red, alpha}.
 *
 * DEVIATION: the previous reconstruction had red and green transposed in the endpoint expansion and invented a
 * "shipped quirk" to explain the resulting asymmetry. The binary stores green into byte 1 (stb r8, +1
 * @0x837E5098, r8 built at 0x837E5078/0x837E507C/0x837E5088) and red into byte 2 (stb r7, +2 @0x837E509C),
 * with endpoint 0 doing the same at 0x837E50F0/0x837E50F4 - the byte order is [B,G,R,0xFF] for every palette
 * entry, and both interpolation paths are byte-parallel (stores 0x837E519C-0x837E51C4). Hex-Rays' BYTE1/BYTE2
 * name little-endian register bytes, which read transposed on this big-endian target. */

#include "headers/s3tc_block.h"
#include <string.h>

void DecodeBlockRGB(S3TCBlockRGB *pblockSrc, S3TC_COLOR *colorDst)
{
    if ( !pblockSrc )
    {
        memset(colorDst, 0, 0x40u);
        return;
    }

    unsigned int rgb1 = pblockSrc->rgb1;
    unsigned int rgb0 = pblockSrc->rgb0;

    /* 4 colors x 4 bytes, big-endian memory order: [blue, green, red, alpha] */
    unsigned char color[16];

    /* endpoint 1 -> palette[1] */
    color[4] = (unsigned char)(((unsigned char)(8 * rgb1) >> 5) | (8 * rgb1));                 /* blue  */
    color[5] = (unsigned char)(((rgb1 >> 9) & 3) | (4 * (rgb1 >> 5)));                         /* green */
    color[6] = (unsigned char)(((unsigned char)(8 * (rgb1 >> 11)) >> 5) | (8 * (rgb1 >> 11))); /* red   */
    color[7] = 0xFF;                                                                           /* alpha */

    /* endpoint 0 -> palette[0] */
    color[0] = (unsigned char)(((unsigned char)(8 * rgb0) >> 5) | (8 * rgb0));
    color[1] = (unsigned char)(((rgb0 >> 9) & 3) | (4 * (rgb0 >> 5)));
    color[2] = (unsigned char)(((unsigned char)(8 * (rgb0 >> 11)) >> 5) | (8 * (rgb0 >> 11)));
    color[3] = 0xFF;

    color[11] = 0xFF;

    if ( rgb1 < rgb0 )
    {
        /* opaque 4-color block: palette[2] = (2*c0 + c1)/3, palette[3] = (c0 + 2*c1)/3, byte-parallel */
        color[8]  = (unsigned char)((2 * color[0] + color[4] + 1) / 3);
        color[9]  = (unsigned char)((2 * color[1] + color[5] + 1) / 3);
        color[10] = (unsigned char)((2 * color[2] + color[6] + 1) / 3);
        color[12] = (unsigned char)((2 * color[4] + color[0] + 1) / 3);
        color[13] = (unsigned char)((2 * color[5] + color[1] + 1) / 3);
        color[14] = (unsigned char)((2 * color[6] + color[2] + 1) / 3);
        color[15] = 0xFF;
    }
    else
    {
        /* 1-bit-alpha 3-color block: palette[2] = (c0 + c1)/2, palette[3] = transparent black */
        for ( int b = 0; b < 3; ++b )
        {
            color[b + 12] = 0;
            color[b + 8] = (unsigned char)((color[b + 4] + color[b]) / 2);
        }
        color[15] = 0;
    }

    unsigned int pixbm = pblockSrc->pixbm;
    unsigned char *out = (unsigned char *)colorDst;
    for ( int i = 0; i < 16; ++i )
    {
        int index = (pixbm & 3) * 4;
        pixbm >>= 2;
        out[0] = color[index];
        out[1] = color[index + 1];
        out[2] = color[index + 2];
        out[3] = color[index + 3];
        out += 4;
    }
}
