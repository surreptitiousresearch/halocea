/* DecodeBlockAlpha4__single_pixel @0x837E5548 — decodes one texel of an S3TC/DXT block with explicit
 * (non-interpolated) 4-bit alpha: delegates the RGB channels to DecodeBlockRGB__single_pixel, then expands
 * the 4-bit alpha nibble to 8 bits by replicating it into both nibbles (0xN -> 0xNN). */

#include <stdint.h>
#include "headers/s3tc_block.h"

#include "headers/S3TCBlockRGB.h"
extern void DecodeBlockRGB__single_pixel(const S3TCBlockRGB *pblockSrc, S3TC_COLOR *colorDst, int16_t u, int16_t v);

void DecodeBlockAlpha4__single_pixel(const S3TCBlockAlpha4 *pblockSrc, S3TC_COLOR *colorDst,
        int16_t u, int16_t v)
{
    DecodeBlockRGB__single_pixel(&pblockSrc->rgb, colorDst, u, v);
    int alpha_nibble = (pblockSrc->alphabm[v] >> (4 * u)) & 0xF;
    colorDst->rgba[3] = (unsigned char)((16 * alpha_nibble) | alpha_nibble);
}
