/* Quantize @0x837E40E8 — quantize the two RGB endpoints of a DXT1/S3TC color block. Each FCOLOR endpoint (a
 * weight-normalized float RGB) is packed to a 16-bit RGB565 value and stored in the block, the two endpoints are
 * ordered per the DXT convention (for a 16/16 opaque block color0 must sort above color1; for a punch-through
 * block the order is flipped), and finally each stored 565 value is dequantized (bit-replicated 5->8 / 6->8 and
 * re-weighted) back into its FCOLOR so the caller sees the exact colors the block will reproduce. Sibling of the
 * already-reversed DecodeBlockRGB.
 *
 * DEVIATION: Hex-Rays emitted "local variable allocation has failed" and merged pfcolor1 and pblock into one
 * OVERLAPPED __int64 (pfcolor1 = HIDWORD, pblock = LODWORD) with heavy register punning through the dequantize
 * math. The DB's 4-arg prototype is authoritative; the packing/expansion bit expressions were verified against
 * the pseudocode (RGB565 with rgba[2]->R, rgba[1]->G, rgba[0]->B; 1/255 == 0.0039215689). wtPrimary is a FCOLOR
 * (Hex-Rays indexed it as a bare float array; wtPrimary[i] == wtPrimary.rgba[i]). */

#include "headers/s3tc_block.h"
#include "headers/FCOLOR.h"
/* wtPrimary here is a DISTINCT file-local object (FCOLOR @0x8422EE00), not the canonical
 * float wtPrimary[3]; kept static to avoid the same-name canonical collision (DB-verified distinct). */
static FCOLOR wtPrimary;
extern const unsigned int mapRGB4[4];    /* selector-index -> DXT1 2-bit code table */


static unsigned __int16 quantize_endpoint(const FCOLOR *color)
{
    int channel2 = (int)(color->rgba[2] / wtPrimary.rgba[2] * 255.0f);
    int channel1 = (int)(color->rgba[1] / wtPrimary.rgba[1] * 255.0f);
    int channel0 = (int)(color->rgba[0] / wtPrimary.rgba[0] * 255.0f);
    return (unsigned __int16)((8 * ((32 * channel2 & 0x1F00) | (unsigned __int8)channel1)) & 0xFFE0
            | ((unsigned __int8)channel0 >> 3));
}

static void dequantize_endpoint(unsigned __int16 color, FCOLOR *result)
{
    unsigned int r5 = color >> 11;
    unsigned int g6 = (color >> 5) & 0x3F;
    unsigned int b5 = color & 0x1F;
    unsigned __int8 r8 = (unsigned __int8)((r5 << 3) | (r5 >> 2));
    unsigned __int8 g8 = (unsigned __int8)((g6 << 2) | (g6 >> 4));
    unsigned __int8 b8 = (unsigned __int8)((b5 << 3) | (b5 >> 2));
    result->rgba[0] = (float)b8 * wtPrimary.rgba[0] * 0.0039215689f;
    result->rgba[1] = (float)g8 * wtPrimary.rgba[1] * 0.0039215689f;
    result->rgba[2] = (float)r8 * wtPrimary.rgba[2] * 0.0039215689f;
}

void Quantize(FCOLOR *pfcolor0, FCOLOR *pfcolor1, S3TCBlockRGB *pblock, int cOpaque)
{
    unsigned __int16 color0 = quantize_endpoint(pfcolor0);
    pblock->rgb0 = color0;
    unsigned __int16 color1 = quantize_endpoint(pfcolor1);
    pblock->rgb1 = color1;

    if ( (cOpaque == 16) != (color1 < color0) )
    {
        pblock->rgb0 = color1;
        pblock->rgb1 = color0;
    }

    dequantize_endpoint(pblock->rgb0, pfcolor0);
    dequantize_endpoint(pblock->rgb1, pfcolor1);
}
