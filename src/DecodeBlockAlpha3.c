/* DecodeBlockAlpha3 @0x837E5598 — decode one S3TC DXT5-style interpolated-alpha block (3-bit indices) into the
 * alpha channel of 16 output pixels (colorDst[0..15]), then decode the embedded RGB sub-block via DecodeBlockRGB.
 * The two endpoint bytes alpha0/alpha1 build an 8-entry alpha lookup table: if alpha0 > alpha1 all six in-between
 * entries are interpolated (a0..a1 in sevenths); otherwise only four are interpolated (in fifths) and the last
 * two entries are the constants 0 and 255. The 16 pixels' 3-bit indices are packed little-endian, 8 pixels per
 * 3-byte group (alphabm[0..2] then alphabm[3..5]); each index selects one table entry.
 *
 * DEVIATION: the decompiler unrolled the per-pixel index fetch four-ways per outer step, emitting three
 * provably-dead refill branches (guarded by i==-1/-2/-3, unreachable for i in {0,4,8,12}). Only pixels 0 and 8
 * ever reload the rolling index window (i&7)==0. Restored the equivalent single per-pixel loop; behaviour is
 * identical. The index window assembles 3 bytes as byte[2]<<16|byte[1]<<8|byte[0] (the decompiler rendered the
 * byte[2]<<8 as __ROL4__(byte,8), equivalent since the operand is a single byte). */

#include <stdint.h>
#include "headers/s3tc_block.h"

extern void DecodeBlockRGB(S3TCBlockRGB *pblockSrc, S3TC_COLOR *colorDst);

void DecodeBlockAlpha3(S3TCBlockAlpha3 *pblockSrc, S3TC_COLOR *colorDst)
{
    DecodeBlockRGB(&pblockSrc->rgb, colorDst);

    int alpha0 = pblockSrc->alpha0;
    int alpha1 = pblockSrc->alpha1;

    int alpha_table[8];
    alpha_table[0] = alpha0;
    alpha_table[1] = alpha1;
    if ( alpha0 <= alpha1 )
    {
        alpha_table[2] = (4 * alpha0 + alpha1) / 5;
        alpha_table[3] = (2 * alpha1 + 3 * alpha0) / 5;
        alpha_table[4] = (3 * alpha1 + 2 * alpha0) / 5;
        alpha_table[5] = (4 * alpha1 + alpha0) / 5;
        alpha_table[6] = 0;
        alpha_table[7] = 255;
    }
    else
    {
        alpha_table[2] = (6 * alpha0 + alpha1) / 7;
        alpha_table[3] = (2 * alpha1 + 5 * alpha0) / 7;
        alpha_table[4] = (3 * alpha1 + 4 * alpha0) / 7;
        alpha_table[5] = (4 * alpha1 + 3 * alpha0) / 7;
        alpha_table[6] = (5 * alpha1 + 2 * alpha0) / 7;
        alpha_table[7] = (6 * alpha1 + alpha0) / 7;
    }

    unsigned int index_window = 0;
    for ( int pixel = 0; pixel < 16; ++pixel )
    {
        if ( (pixel & 7) == 0 )
        {
            int base = pixel ? 3 : 0;
            index_window = ((unsigned int)pblockSrc->alphabm[base + 2] << 16)
                    | ((unsigned int)pblockSrc->alphabm[base + 1] << 8)
                    | pblockSrc->alphabm[base];
        }
        colorDst[pixel].rgba[3] = (uint8_t)alpha_table[index_window & 7];
        index_window >>= 3;
    }
}
