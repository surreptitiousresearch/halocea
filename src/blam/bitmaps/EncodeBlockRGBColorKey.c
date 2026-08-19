/* EncodeBlockRGBColorKey @0x837E4640 — encode 16 source texels into a DXT1/S3TC RGB color block using
 * principal-component analysis. Weight and normalize each texel's RGB, find the block centroid, build the
 * 3x3 covariance matrix of the centered texels, extract the dominant color axis by power-iteration
 * (repeated matrix squaring), project the texels onto that axis to obtain the two extreme endpoints, clip
 * and quantize them to RGB565 (via ClipExtrema/Quantize), then assign every texel a 2-bit selector by its
 * position along the endpoint line and pack the selectors into pblockDst->pixbm. Degenerate (zero-variance)
 * blocks fall back to AllSame(). Sibling of the already-reversed Quantize/ClipExtrema/AllSame and the
 * DecodeBlockRGB decoder; called by EncodeBlockAlpha3/EncodeBlockAlpha4 for the RGB sub-block.
 *
 * CAVEAT / deviations (Hex-Rays reported "local variable allocation has failed"; verified against disasm
 * 0x837E4640-0x837E5004):
 *  - The third parameter (colorKey) is part of the ABI prototype the callers use, but the body never reads
 *    r5: the opaque-texel mask is built internally as a constant 0xFFFF (all 16 texels present). The
 *    per-texel "transparent -> selector 3" branch and the masked accumulations are therefore preserved for
 *    fidelity but are effectively always-taken/always-full here.
 *  - The stack working buffer is a planar 16x(4-float) region that Hex-Rays split across several byte-array
 *    locals with overlapped pointer arithmetic. It is re-modeled here as float weightedPixels[16][4]
 *    (channels 0..2 = R/G/B, slot 3 = padding), matching the real 16-byte-per-texel stride; the load,
 *    mean, centering, covariance and projection loops are rewritten as ordinary per-texel loops.
 *  - The covariance matrix is squared each power-iteration step; the exact per-entry arithmetic of the
 *    symmetric 3x3 square (and the 3/trace renormalization) is transcribed verbatim into named scalars.
 *  - __fsqrts is the PPC reciprocal-friendly single-precision sqrt intrinsic (endpoint-axis normalization
 *    divides by sqrt(largest diagonal), as the original does — not by the true vector norm).
 *  - 0.0039215689f == 1/255. */

#include <stdint.h>
#include "headers/s3tc_block.h"
#include "headers/fcolor.h"
/* wtPrimary here is a DISTINCT file-local object (FCOLOR @0x8422EE00), not the canonical
 * float wtPrimary[3]; kept static to avoid the same-name canonical collision (DB-verified distinct). */
static FCOLOR wtPrimary;
extern const unsigned int mapRGB4[4];    /* selector-index -> DXT1 2-bit code table (canonical global) */

extern float __fsqrts(float);            /* PPC single-precision sqrt intrinsic */

extern void ClipExtrema(FCOLOR *plower, FCOLOR *pupper);
extern void Quantize(FCOLOR *pfcolor0, FCOLOR *pfcolor1, S3TCBlockRGB *pblock, int cOpaque);
extern void AllSame(S3TC_COLOR *pcolor, S3TCBlockRGB *pblock, uint16_t wAlpha);

void EncodeBlockRGBColorKey(S3TC_COLOR *colorSrc, S3TCBlockRGB *pblockDst, int colorKey)
{
    (void)colorKey;   /* unused by the body; opaque-texel mask is built as all-present below */

    if ( !pblockDst )
        return;

    /* Opaque-texel mask — built to 0xFFFF (all 16 texels present). */
    uint16_t pixelMask = 0;
    for ( int bit = 0; bit < 16; ++bit )
        pixelMask = (uint16_t)((2 * pixelMask) | 1);

    /* 1) Load 16 texels, weight each channel by wtPrimary and normalize to [0,1]. */
    float weightedPixels[16][4];
    for ( int pixel = 0; pixel < 16; ++pixel )
    {
        weightedPixels[pixel][0] = (float)colorSrc[pixel].rgba[0] * wtPrimary.rgba[0] * 0.0039215689f;
        weightedPixels[pixel][1] = (float)colorSrc[pixel].rgba[1] * wtPrimary.rgba[1] * 0.0039215689f;
        weightedPixels[pixel][2] = (float)colorSrc[pixel].rgba[2] * wtPrimary.rgba[2] * 0.0039215689f;
    }

    /* 2) Block centroid (fixed 1/16 divisor, matching the reference encoder). */
    FCOLOR meanColor;
    for ( int channel = 0; channel < 3; ++channel )
    {
        float channelSum = 0.0f;
        for ( int pixel = 0; pixel < 16; ++pixel )
            if ( ((1u << pixel) & pixelMask) != 0 )
                channelSum += weightedPixels[pixel][channel];
        meanColor.rgba[channel] = channelSum * 0.0625f;
    }

    /* 3) Center the texels about the mean. */
    for ( int channel = 0; channel < 3; ++channel )
        for ( int pixel = 0; pixel < 16; ++pixel )
            weightedPixels[pixel][channel] -= meanColor.rgba[channel];

    /* 4) Symmetric 3x3 covariance matrix of the centered texels. */
    float cov00 = 0.0f, cov01 = 0.0f, cov02 = 0.0f, cov11 = 0.0f, cov12 = 0.0f, cov22 = 0.0f;
    for ( int pixel = 0; pixel < 16; ++pixel )
        if ( ((1u << pixel) & pixelMask) != 0 )
        {
            float centered_r = weightedPixels[pixel][0];
            float centered_g = weightedPixels[pixel][1];
            float centered_b = weightedPixels[pixel][2];
            cov00 += centered_r * centered_r;
            cov01 += centered_r * centered_g;
            cov02 += centered_r * centered_b;
            cov11 += centered_g * centered_g;
            cov12 += centered_g * centered_b;
            cov22 += centered_b * centered_b;
        }

    /* 5) Power iteration: square the symmetric covariance matrix so its columns converge to the
     *    dominant eigenvector (the principal color axis). */
    for ( int iteration = 0; iteration < 9; ++iteration )
    {
        float cross12 = (cov22 + cov11) * cov12 + cov02 * cov01;
        float cross01 = (cov11 + cov00) * cov01 + cov12 * cov02;
        float cross02 = (cov22 + cov00) * cov02 + cov12 * cov01;
        float diag0 = (cov11 * cov11 + cov12 * cov12) + cov01 * cov01;
        float diag1 = (cov22 * cov22 + cov12 * cov12) + cov02 * cov02;
        float diag2 = (cov00 * cov00 + cov01 * cov01) + cov02 * cov02;

        float square00 = (diag2 * diag2 + cross01 * cross01) + cross02 * cross02;
        float square11 = (diag0 * diag0 + cross12 * cross12) + cross01 * cross01;
        float square22 = (diag1 * diag1 + cross12 * cross12) + cross02 * cross02;
        float square01 = (diag0 + diag2) * cross01 + cross12 * cross02;
        float square02 = (diag1 + diag2) * cross02 + cross12 * cross01;
        float square12 = (diag1 + diag0) * cross12 + cross02 * cross01;

        cov00 = square00; cov01 = square01; cov02 = square02;
        cov11 = square11; cov12 = square12; cov22 = square22;

        /* Zero-variance block -> nothing to interpolate. */
        if ( (cov22 + cov11) + cov00 == 0.0f )
        {
            AllSame(colorSrc, pblockDst, pixelMask);
            return;
        }

        /* Renormalize by 3/trace to keep the magnitudes bounded across iterations. */
        float invTraceScale = 3.0f / ((cov22 + cov11) + cov00);
        cov00 *= invTraceScale; cov01 *= invTraceScale; cov02 *= invTraceScale;
        cov11 *= invTraceScale; cov12 *= invTraceScale; cov22 *= invTraceScale;
    }

    /* 6) Principal axis = column of the largest diagonal, normalized by sqrt(that diagonal). */
    float axisMatrix[3][3];
    axisMatrix[0][0] = cov00; axisMatrix[0][1] = cov01; axisMatrix[0][2] = cov02;
    axisMatrix[1][0] = cov01; axisMatrix[1][1] = cov11; axisMatrix[1][2] = cov12;
    axisMatrix[2][0] = cov02; axisMatrix[2][1] = cov12; axisMatrix[2][2] = cov22;

    float bestDiagonal = 0.0f;
    int axisColumn = 0;   /* decompiler leaves this uninitialized when cov00 <= 0; trace > 0 always resets it */
    if ( cov00 > 0.0f ) { bestDiagonal = cov00; axisColumn = 0; }
    if ( cov11 > bestDiagonal ) { bestDiagonal = cov11; axisColumn = 1; }
    if ( cov22 > bestDiagonal ) { bestDiagonal = cov22; axisColumn = 2; }

    float invAxisLen = 1.0f / __fsqrts(bestDiagonal);
    float axisR = axisMatrix[0][axisColumn] * invAxisLen;
    float axisG = axisMatrix[1][axisColumn] * invAxisLen;
    float axisB = axisMatrix[2][axisColumn] * invAxisLen;

    if ( (axisG * axisG + (axisR * axisR + axisB * axisB)) == 0.0f )
    {
        AllSame(colorSrc, pblockDst, pixelMask);
        return;
    }

    /* 7) Project the centered texels onto the axis; track the extreme projections. */
    float axisLenSq = axisG * axisG + (axisR * axisR + axisB * axisB);
    float minProjection = 99999.0f;
    float maxProjection = -99999.0f;
    for ( int pixel = 0; pixel < 16; ++pixel )
        if ( ((1u << pixel) & pixelMask) != 0 )
        {
            float projection = (axisR * weightedPixels[pixel][0]
                              + axisG * weightedPixels[pixel][1]
                              + axisB * weightedPixels[pixel][2]) / axisLenSq;
            if ( projection < minProjection ) minProjection = projection;
            if ( projection > maxProjection ) maxProjection = projection;
        }

    /* 8) Endpoints = mean +/- axis*projection, then clip and quantize to RGB565. */
    float meanR = meanColor.rgba[0];
    float meanG = meanColor.rgba[1];
    float meanB = meanColor.rgba[2];

    FCOLOR minEndpoint, maxEndpoint;
    maxEndpoint.rgba[0] = axisR * maxProjection + meanR;
    minEndpoint.rgba[0] = axisR * minProjection + meanR;
    maxEndpoint.rgba[1] = axisG * maxProjection + meanG;
    minEndpoint.rgba[1] = axisG * minProjection + meanG;
    maxEndpoint.rgba[2] = axisB * maxProjection + meanB;
    minEndpoint.rgba[2] = axisB * minProjection + meanB;

    ClipExtrema(&minEndpoint, &maxEndpoint);
    Quantize(&minEndpoint, &maxEndpoint, pblockDst, 16);

    /* 9) Assign each texel a 2-bit selector from its position along the (clipped) endpoint line. */
    float lowerR = minEndpoint.rgba[0];
    float lowerG = minEndpoint.rgba[1];
    float lowerB = minEndpoint.rgba[2];
    float deltaR = maxEndpoint.rgba[0] - minEndpoint.rgba[0];
    float deltaG = maxEndpoint.rgba[1] - minEndpoint.rgba[1];
    float deltaB = maxEndpoint.rgba[2] - minEndpoint.rgba[2];
    float lineLenSq = deltaB * deltaB + (deltaR * deltaR + deltaG * deltaG);

    if ( lineLenSq == 0.0f )
    {
        AllSame(colorSrc, pblockDst, pixelMask);
        return;
    }

    /* Texels are packed MSB-first (texel 15 down to texel 0), 2 bits each. */
    for ( int pixel = 15; pixel >= 0; --pixel )
    {
        if ( ((1u << pixel) & pixelMask) != 0 )
        {
            /* Buffer holds centered data; restore the absolute weighted color. (The reference encoder
             * also writes the restored color back into the scratch buffer, which is dead afterwards.) */
            float texelR = meanR + weightedPixels[pixel][0];
            float texelG = meanG + weightedPixels[pixel][1];
            float texelB = meanB + weightedPixels[pixel][2];

            float position = (((texelB - lowerB) * deltaB
                             + ((texelR - lowerR) * deltaR + (texelG - lowerG) * deltaG)) / lineLenSq) * 4.0f;
            if ( position >= 0.0f )
            {
                if ( position >= 4.0f )
                    position = 3.0f;
            }
            else
            {
                position = 0.0f;
            }

            int selectorIndex = (int)position;
            pblockDst->pixbm = mapRGB4[selectorIndex] | (4 * pblockDst->pixbm);
        }
        else
        {
            /* Color-key transparent texel -> DXT1 punch-through selector 3. */
            pblockDst->pixbm = (4 * pblockDst->pixbm) | 3;
        }
    }
}
