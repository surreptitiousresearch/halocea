/* DecodeBlockRGB__single_pixel @0x837E5260 — decodes one texel's RGB from an S3TC/DXT RGB565 block: builds
 * the standard 2 (or 4) entry DXT1 color palette and looks up the pixel's 2-bit index. A null block clears
 * a full 4x4-tile output (0x40 bytes) rather than a single pixel — preserved as found.
 *
 * DEVIATION (verified via disasm, not decompiler text — the decompiler's own BYTE1/BYTE2 labels for this
 * function are unreliable): each palette entry is stored as 4 bytes in **[Blue, Red, Green, Alpha]** order
 * (a consequence of building each color as a single big-endian 32-bit value via HIBYTE/BYTE2/BYTE1/LOBYTE
 * inserts, then reading it back through a byte pointer). disasm_range(0x837E5394, 0x837E5420) shows that in
 * the opaque 4-color palette, color2's BLUE channel is a plain (blue0+blue1+1)/3 average — NOT the 2:1
 * weighted (2*blue0+blue1+1)/3 used for its own red/green channels and for all three of color3's channels.
 * This asymmetry is reproduced faithfully as an authentic quirk of the compiled code, not "corrected" to
 * the textbook DXT1 formula. */

#include <stdint.h>
#include <string.h>

#include "headers/s3tc_block.h"

static unsigned char expand5(unsigned int five_bit_value)
{
    unsigned int v = five_bit_value & 0x1F;
    return (unsigned char)((v << 3) | (v >> 2));
}

static unsigned char expand6(unsigned int six_bit_value)
{
    unsigned int v = six_bit_value & 0x3F;
    return (unsigned char)((v << 2) | (v >> 4));
}

void DecodeBlockRGB__single_pixel(const S3TCBlockRGB *pblockSrc, S3TC_COLOR *colorDst, int16_t u, int16_t v)
{
    if (!pblockSrc)
    {
        memset(colorDst, 0, 0x40);
        return;
    }

    unsigned int rgb1 = pblockSrc->rgb1;
    unsigned int rgb0 = pblockSrc->rgb0;

    unsigned char blue0 = expand5(rgb0);
    unsigned char red0 = expand5(rgb0 >> 11);
    unsigned char green0 = expand6(rgb0 >> 5);
    unsigned char blue1 = expand5(rgb1);
    unsigned char red1 = expand5(rgb1 >> 11);
    unsigned char green1 = expand6(rgb1 >> 5);

    /* palette[n] = { blue, red, green, alpha } */
    unsigned char palette[4][4];
    palette[0][0] = blue0;  palette[0][1] = red0;  palette[0][2] = green0;  palette[0][3] = 255;
    palette[1][0] = blue1;  palette[1][1] = red1;  palette[1][2] = green1;  palette[1][3] = 255;

    if (rgb1 < rgb0)
    {
        palette[2][0] = (unsigned char)((blue0 + blue1 + 1) / 3); /* unweighted — faithful quirk, see above */
        palette[2][1] = (unsigned char)((2 * red0 + red1 + 1) / 3);
        palette[2][2] = (unsigned char)((2 * green0 + green1 + 1) / 3);
        palette[2][3] = 255;
        palette[3][0] = (unsigned char)((2 * blue1 + blue0 + 1) / 3);
        palette[3][1] = (unsigned char)((2 * red1 + red0 + 1) / 3);
        palette[3][2] = (unsigned char)((2 * green1 + green0 + 1) / 3);
        palette[3][3] = 255;
    }
    else
    {
        palette[2][0] = (unsigned char)((blue0 + blue1) / 2);
        palette[2][1] = (unsigned char)((red0 + red1) / 2);
        palette[2][2] = (unsigned char)((green0 + green1) / 2);
        palette[2][3] = 255;
        palette[3][0] = 0;
        palette[3][1] = 0;
        palette[3][2] = 0;
        palette[3][3] = 0;
    }

    unsigned int index = (pblockSrc->pixbm >> (2 * (4 * v + u))) & 3;
    colorDst->rgba[0] = palette[index][0];
    colorDst->rgba[1] = palette[index][1];
    colorDst->rgba[2] = palette[index][2];
    colorDst->rgba[3] = palette[index][3];
}
