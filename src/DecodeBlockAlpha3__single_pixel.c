/* DecodeBlockAlpha3__single_pixel @0x837E5848 — decodes one texel of an S3TC/DXT block with 3-bit
 * interpolated alpha (the DXT5 alpha scheme): delegates RGB to DecodeBlockRGB__single_pixel, builds the
 * 8-entry alpha ramp (alpha0, alpha1, then either a 6-value linear ramp + explicit 0/255, or a 8-value
 * linear ramp, depending on alpha0 vs alpha1), and looks up the pixel's 3-bit index from the 48-bit index
 * bitmap (two 24-bit halves, one per pixel row-pair, 3 bits per pixel in row-major order).
 *
 * DEVIATION: reproduced the `__ROL4__(byte, 8) | byte) << 8 | byte` index-bitmap idiom as its equivalent
 * little-endian byte assembly (each rotated/shifted byte carries no set bits above bit 7, so the rotate is
 * a plain shift here); verified against disasm's byte-by-byte construction. */

#include <stdint.h>
#include "headers/s3tc_block.h"

/* recovered: removed duplicate inline S3TCBlockAlpha3 body -> provided by headers/s3tc_block.h */

#include "headers/S3TCBlockRGB.h"
extern void DecodeBlockRGB__single_pixel(const S3TCBlockRGB *pblockSrc, S3TC_COLOR *colorDst, int16_t u, int16_t v);

void DecodeBlockAlpha3__single_pixel(const S3TCBlockAlpha3 *pblockSrc, S3TC_COLOR *colorDst,
        int16_t u, int16_t v)
{
    DecodeBlockRGB__single_pixel(&pblockSrc->rgb, colorDst, u, v);

    unsigned int alpha0 = pblockSrc->alpha0;
    unsigned int alpha1 = pblockSrc->alpha1;
    uint16_t alpha_table[8];
    alpha_table[0] = (uint16_t)alpha0;
    alpha_table[1] = (uint16_t)alpha1;

    if (alpha0 <= alpha1)
    {
        alpha_table[2] = (4 * alpha0 + alpha1) / 5;
        alpha_table[3] = (3 * alpha0 + 2 * alpha1) / 5;
        alpha_table[4] = (2 * alpha0 + 3 * alpha1) / 5;
        alpha_table[5] = (alpha0 + 4 * alpha1) / 5;
        alpha_table[6] = 0;
        alpha_table[7] = 255;
    }
    else
    {
        alpha_table[2] = (6 * alpha0 + alpha1) / 7;
        alpha_table[3] = (5 * alpha0 + 2 * alpha1) / 7;
        alpha_table[4] = (4 * alpha0 + 3 * alpha1) / 7;
        alpha_table[5] = (3 * alpha0 + 4 * alpha1) / 7;
        alpha_table[6] = (2 * alpha0 + 5 * alpha1) / 7;
        alpha_table[7] = (alpha0 + 6 * alpha1) / 7;
    }

    unsigned int bits;
    int shift;
    if (v >= 2)
    {
        bits = pblockSrc->alphabm[3] | (pblockSrc->alphabm[4] << 8) | (pblockSrc->alphabm[5] << 16);
        shift = 3 * (4 * (v - 2) + u);
    }
    else
    {
        bits = pblockSrc->alphabm[0] | (pblockSrc->alphabm[1] << 8) | (pblockSrc->alphabm[2] << 16);
        shift = 3 * (4 * v + u);
    }

    unsigned int index = (bits >> shift) & 7;
    colorDst->rgba[3] = (unsigned char)alpha_table[index];
}
