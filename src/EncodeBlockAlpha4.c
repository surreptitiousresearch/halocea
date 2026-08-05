/* EncodeBlockAlpha4 @0x837E5A20 — encode 16 source pixels' alpha channel into an S3TC DXT5-style 4-bit alpha
 * block (inverse of DecodeBlockAlpha4.c), then encode the block's embedded RGB sub-block via
 * EncodeBlockRGBColorKey. Each output row packs 4 pixels' alpha into 4-bit nibbles (only the top 4 bits of
 * each 8-bit alpha value are kept, matching DecodeBlockAlpha4's (nibble<<4)|nibble expansion), with pixel 0
 * of each row of 4 landing in the low nibble.
 *
 * DEVIATION: the decompiler unrolled this into a long chain of per-pixel shift/mask/OR sequences using
 * __ROL4__ and raw bit twiddling (clrlslwi/clrrwi at the disasm level). Traced through disasm
 * (0x837E5A20-0x837E5BC8) and confirmed it implements the standard "shift accumulator left 4, OR in the
 * next nibble" packing, processing each row's 4 pixels from index 3 down to 0 — restored as the equivalent
 * loop. */

#include <stdint.h>
#include "headers/s3tc_block.h"

extern void EncodeBlockRGBColorKey(S3TC_COLOR *colorSrc, S3TCBlockRGB *pblockDst, int colorKey);

void EncodeBlockAlpha4(S3TC_COLOR *colorSrc, S3TCBlockAlpha4 *pblockDst)
{
    for (int row = 0; row < 4; row++)
    {
        uint16_t packed = 0;
        for (int col = 3; col >= 0; col--)
            packed = (uint16_t)((packed << 4) | ((colorSrc[row * 4 + col].rgba[3] >> 4) & 0xF));
        pblockDst->alphabm[row] = packed;
    }

    EncodeBlockRGBColorKey(colorSrc, &pblockDst->rgb, 0);
}
