/* EncodeBlockAlpha3 @0x837E5BD0 — compress the alpha channel of a 4x4 pixel block into a DXT5/S3TC
 * 3-bit-alpha block (two 8-bit alpha endpoints + a 3-bit index per pixel), then hand the color channels to
 * EncodeBlockRGBColorKey. Two endpoint modes:
 *   - 8-alpha mode (alpha0 > alpha1): endpoints are the block's max/min alpha; all 8 index codes interpolate.
 *   - 6-alpha mode (alpha0 < alpha1): used only when the block contains BOTH 0 and 255. Endpoints bracket the
 *     interior (non-0, non-255) alphas; codes 0..5 interpolate, code 6 = 0, code 7 = 255. Falls back to
 *     8-alpha (255/0) when there is no interior range.
 * The per-pixel index is idx = ((alpha0 - alpha)*steps + range/2) / range, remapped so idx 0 -> code 0,
 * idx >= steps -> code 1, else code idx+1 (steps = 7 in 8-alpha mode, 5 in 6-alpha mode). range = alpha0 -
 * alpha1 (negative in 6-alpha mode, which flips the division sign consistently). Indices pack LSB-first, three
 * bits per pixel: pixels 0..7 into alphabm[0..2], pixels 8..15 into alphabm[3..5], each little-endian.
 *
 * DEVIATIONS: the decompiler unrolled the pack loop 4x and rendered the byte extraction with spurious
 * BYTE2/>>16 register recycling; disasm (0x837E5EB0-0x837E5EDC etc.) shows each 8-pixel half stores plainly as
 * {acc & 0xFF, (acc>>8) & 0xFF, (acc>>16) & 0xFF}. The min/max scans are order-independent, reconstructed as
 * simple loops. All decodes disasm-verified 0x837E5BD0-0x837E611C. */

#include "headers/s3tc_block.h"

extern void EncodeBlockRGBColorKey(S3TC_COLOR *colorSrc, S3TCBlockRGB *pblockDst, int colorKey);

/* 3-bit index code for one pixel's alpha (see formula above). range/bias are signed; both flip sign together
 * in 6-alpha mode so the truncating division still maps alpha0->0, alpha1->steps. */
static int encode_block_alpha3_code(int alpha, int alpha0, int range, int bias, int steps, int has_extremes)
{
    if ( has_extremes )
    {
        if ( alpha == 0 )
            return 6;
        if ( alpha == 255 )
            return 7;
    }
    int idx = ((alpha0 - alpha) * steps + bias) / range;
    if ( idx < steps )
    {
        if ( idx > 0 )
            return idx + 1;
        return 0;
    }
    return 1;
}

void EncodeBlockAlpha3(S3TC_COLOR *colorSrc, S3TCBlockAlpha3 *pblockDst)
{
    /* block-wide alpha max (-> endpoint candidate) and min */
    int max_alpha = colorSrc[0].rgba[3];
    int min_alpha = max_alpha;
    for ( int i = 0; i < 16; ++i )
    {
        int alpha = colorSrc[i].rgba[3];
        if ( alpha > max_alpha )
            max_alpha = alpha;
        if ( alpha < min_alpha )
            min_alpha = alpha;
    }

    int has_extremes;
    if ( max_alpha != 255 || min_alpha != 0 )
    {
        has_extremes = 0;
    }
    else
    {
        /* block spans 0..255: re-scan for the interior range, excluding 0 and 255 */
        for ( int i = 0; i < 16; ++i )
        {
            int alpha = colorSrc[i].rgba[3];
            if ( alpha < max_alpha && alpha != 0 )
                max_alpha = alpha;
            if ( alpha > min_alpha && alpha != 255 )
                min_alpha = alpha;
        }
        if ( max_alpha >= min_alpha )
        {
            has_extremes = 0;
            max_alpha = 255;
            min_alpha = 0;
        }
        else
        {
            has_extremes = 1;
        }
    }

    pblockDst->alpha0 = max_alpha;
    pblockDst->alpha1 = min_alpha;

    if ( max_alpha == min_alpha )
    {
        for ( int i = 0; i < 6; ++i )
            pblockDst->alphabm[i] = 0;
        EncodeBlockRGBColorKey(colorSrc, &pblockDst->rgb, 0);
        return;
    }

    int alpha0 = max_alpha;
    int range = max_alpha - min_alpha;
    int bias = range >> 1;
    int steps = has_extremes ? 5 : 7;

    /* pixels 8..15 -> alphabm[3..5], pixels 0..7 -> alphabm[0..2], each three bits LSB-first, little-endian */
    unsigned int acc = 0;
    for ( int pixel = 15; pixel >= 8; --pixel )
        acc = (acc << 3) | encode_block_alpha3_code(colorSrc[pixel].rgba[3], alpha0, range, bias, steps,
                has_extremes);
    pblockDst->alphabm[3] = acc & 0xFF;
    pblockDst->alphabm[4] = (acc >> 8) & 0xFF;
    pblockDst->alphabm[5] = (acc >> 16) & 0xFF;

    acc = 0;
    for ( int pixel = 7; pixel >= 0; --pixel )
        acc = (acc << 3) | encode_block_alpha3_code(colorSrc[pixel].rgba[3], alpha0, range, bias, steps,
                has_extremes);
    pblockDst->alphabm[0] = acc & 0xFF;
    pblockDst->alphabm[1] = (acc >> 8) & 0xFF;
    pblockDst->alphabm[2] = (acc >> 16) & 0xFF;

    EncodeBlockRGBColorKey(colorSrc, &pblockDst->rgb, 0);
}
