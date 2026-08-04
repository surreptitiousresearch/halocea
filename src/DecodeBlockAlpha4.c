/* DecodeBlockAlpha4 @0x837E54A8 — decode one S3TC DXT5-style 4-bit alpha block into the alpha channel of 16
 * output pixels (colorDst[0..15]), then decode the block's embedded RGB sub-block via DecodeBlockRGB. Each of
 * the four 16-bit alpha rows holds four 4-bit nibbles (nibble 0 = least significant); each nibble is expanded
 * to a full byte as (nibble<<4)|nibble so 0x0-0xF maps evenly across 0x00-0xFF.
 *
 * DEVIATION: the decompiler expressed the pixel/row addressing as pointer arithmetic starting from
 * &pblockSrc[-1].rgb.pixbm and &colorDst[-1]+3, advancing by fixed byte strides per nibble/row. Hand-traced
 * the resulting absolute offsets (confirmed row i writes colorDst[4*i..4*i+3]) and restored the equivalent
 * direct row/column indexing. */

#include "headers/s3tc_block.h"

extern void DecodeBlockRGB(S3TCBlockRGB *pblockSrc, S3TC_COLOR *colorDst);

void DecodeBlockAlpha4(S3TCBlockAlpha4 *pblockSrc, S3TC_COLOR *colorDst)
{
    DecodeBlockRGB(&pblockSrc->rgb, colorDst);

    for (int row = 0; row < 4; row++)
    {
        unsigned __int16 packed = pblockSrc->alphabm[row];
        S3TC_COLOR *pixel = &colorDst[row * 4];

        pixel[0].rgba[3] = (unsigned __int8)((16 * (packed & 0xF)) | (packed & 0xF));
        pixel[1].rgba[3] = (unsigned __int8)((16 * ((packed >> 4) & 0xF)) | ((packed >> 4) & 0xF));
        pixel[2].rgba[3] = (unsigned __int8)((16 * ((packed >> 8) & 0xF)) | ((packed >> 8) & 0xF));
        pixel[3].rgba[3] = (unsigned __int8)((16 * (packed >> 12)) | (packed >> 12));
    }
}
