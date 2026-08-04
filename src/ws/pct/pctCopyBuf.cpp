// pctCopyBuf @0x827C1E70 — copy one face/mip of a picture into a caller-supplied linear buffer,
// converting to a destination pixel format (pDstFmt) and honoring row `pitch` / `slicePitch`.
//
// Handles four cases:
//   1. source DXT + dest DXT      -> pctCopyDXTBuf (block passthrough).
//   2. both linear-addressable (state bit 0x20) and identical format -> plain row memcpy;
//      same bit-depth but differing swizzle -> pctSwizzleBox / pctUnSwizzleBox.
//   3. dest linear, per-pixel channel-remap: decode each source texel (DXT / palette / raw mask
//      unpack), rescale each channel from source to dest bit-depth, repack into pDstFmt.
//   4. dest DXT (from non-DXT source): unsupported here -> silently skipped.
//
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
//
// DEVIATION: the decompiler flagged "local variable allocation has failed"; the per-channel
// scale-factor computation was heavily register-aliased. It is reconstructed here to its evident
// semantic — scale[ch] = maxDstValue[ch] / maxSrcValue[ch], where max = (1<<bitLength)-1 and a
// DXT source is treated as 8-bit (max 255). Channel<->length index map (verified from the pack
// path's shift[] usage): r=1, g=2, b=3, a=0.
#include "../../headers/pctPICTURE.h"
#include "../../headers/pctFORMAT.h"
#include "../../headers/ws/gs/gsRENDER_SYSTEM_boundary.h" // pctPIXEL definition
#include "../../headers/pct_boundary.h"                    // pctGetFormat, pctGetMipMapOffset
#include "../../headers/ws/ds/ds_assert_boundary.h"        // STRONG_ASSERT
#include <string.h>                                        // memcpy — CRT boundary

// --- pct/m3d helpers (reversed elsewhere / lower level). boundary decls. ---
extern void pctCopyDXTBuf(int w, int h, unsigned char *src, int srcFormat, int flag,
                          unsigned char *dst, int dstFormatIndex, int pitch, int unused);
extern void pctUnSwizzleBox(unsigned char *src, int w, int h, int d, unsigned char *dst, int bpp, int depth);
extern void pctSwizzleBox(unsigned char *src, unsigned char *dst, int w, int h, int d, int bpp, int depth);
extern void pctGetDXTPixel(pctPICTURE *pPict, int x, int y, int mip, pctPIXEL *pPix, int z, int face);
extern void pctGetPixel(pctPICTURE *pPict, int x, int y, int mip, pctPIXEL *pPix, int z, int face);
extern int  pctGetSwizzleOffs(int w, int h, int bitpp, int x, int y);
extern unsigned int m3dMaskBuffer2DWordReverce(int bpp, unsigned char *pSrc);
extern void _blkmov(void *dst, const void *src, int n); // PPC block-move intrinsic wrapper. boundary.

// maximum representable value for a channel of the given bit length ((1<<len)-1)
static inline int pctChannelMax(int bitLength)
{
    return (1 << bitLength) - 1;
}

void pctCopyBuf(pctPICTURE *pPict, int face, int mipMapNmb, pctFORMAT *pDstFmt, int pitch,
                unsigned char *dstBuffer, int slicePitch)
{
    float scaleR = 0.0f, scaleG = 0.0f, scaleB = 0.0f, scaleA = 0.0f;

    pctFORMAT *srcFormat = pctGetFormat(pPict->hdr.format);
    int sz = pPict->hdr.sz;
    int w = pPict->hdr.sx >> mipMapNmb;
    int h = pPict->hdr.sy >> mipMapNmb;
    int depth = sz >> mipMapNmb;
    if (sz <= 1)
        depth = 1;

    int mipOffset = pctGetMipMapOffset(&pPict->hdr, mipMapNmb);
    int srcState = srcFormat->state;
    unsigned char *srcBase = (unsigned char *)pPict->pFaces[face] + mipOffset;

    // ---- case 1: source and destination both block-compressed (DXT) ----
    if ((srcState & 8) != 0 && (pDstFmt->state & 8) != 0)
    {
        // resolve pDstFmt's index within the global format table (116-byte entries)
        int dstFormatIndex = 0;
        pctFORMAT *pScan = pctFormatList;
        int byteOffset = 0;
        while (pScan != pDstFmt)
        {
            byteOffset += 116;
            ++dstFormatIndex;
            ++pScan;
            if (byteOffset >= 4408) // table exhausted -> not found
            {
                dstFormatIndex = -1;
                break;
            }
        }
        pctCopyDXTBuf(w, h, srcBase, pPict->hdr.format, -1, dstBuffer, dstFormatIndex, pitch, 0);
        return;
    }

    // dest DXT (from non-DXT source) is unsupported: skip entirely.
    if ((pDstFmt->state & 8) != 0)
        return;

    int dstState = pDstFmt->state;

    // ---- case 2: both formats linear-addressable (bit 0x20) ----
    if ((srcState & 0x20) != 0 && (dstState & 0x20) != 0)
    {
        if (srcFormat == pDstFmt)
        {
            // identical format: straight per-row byte copy
            for (int row = h; row > 0; --row)
            {
                unsigned int rowBytes = srcFormat->bitpp * w / 8;
                memcpy(dstBuffer, srcBase, rowBytes);
                dstBuffer += pitch;
                srcBase += srcFormat->bitpp * w / 8;
            }
        }
        else if (srcFormat->bitpp == pDstFmt->bitpp)
        {
            // same bit-depth, differing swizzle
            if ((srcState & 0x40) != 0)
                pctUnSwizzleBox(srcBase, w, h, 1, dstBuffer, pDstFmt->bpp, 1);
            else if ((dstState & 0x40) != 0)
                pctSwizzleBox(srcBase, dstBuffer, w, h, 1, pDstFmt->bpp, 1);
        }
        return;
    }

    // ---- case 3: dest is linear (not 0x20): per-pixel channel remap ----
    if ((dstState & 0x20) != 0)
        return; // dest 0x20 with non-0x20 source: unsupported, skip

    bool sameFormat = (pDstFmt == srcFormat);
    if (!sameFormat)
    {
        // Reconstructed per-channel rescale factors (see file header DEVIATION note).
        if ((srcState & 8) != 0)
        {
            // DXT source decodes to 8-bit channels (src max = 255)
            scaleR = (float)pctChannelMax(pDstFmt->length[1]) * 0.0039215689f; // /255
            scaleG = (float)pctChannelMax(pDstFmt->length[2]) * 0.0039215689f;
            scaleB = (float)pctChannelMax(pDstFmt->length[3]) * 0.0039215689f;
            scaleA = (float)pctChannelMax(pDstFmt->length[0]) * 0.0039215689f;
        }
        else
        {
            scaleR = (float)pctChannelMax(pDstFmt->length[1]) / (float)pctChannelMax(srcFormat->length[1]);
            scaleG = (float)pctChannelMax(pDstFmt->length[2]) / (float)pctChannelMax(srcFormat->length[2]);
            scaleB = (float)pctChannelMax(pDstFmt->length[3]) / (float)pctChannelMax(srcFormat->length[3]);
            if (srcFormat->length[0])
                scaleA = (float)pctChannelMax(pDstFmt->length[0]) / (float)pctChannelMax(srcFormat->length[0]);
            else
                scaleA = 1.0f; // source has no alpha channel
        }
    }

    unsigned char *slicePtr = dstBuffer;
    if (depth <= 0)
        return;

    for (int slice = 0; slice < depth; ++slice)
    {
        unsigned char *rowPtr = slicePtr;
        for (int i = 0; i < h; ++i)
        {
            if (sameFormat)
            {
                // identical format: memcpy the whole row
                memcpy(rowPtr, srcBase, srcFormat->bpp * w);
                rowPtr += pitch;
                srcBase += srcFormat->bpp * w;
                continue;
            }

            for (int j = 0; j < w; ++j)
            {
                int rOut, gOut, bOut, aOut; // pre-shift, pre-add channel contributions
                int srcPixState = srcFormat->state;

                if ((srcPixState & 8) != 0)
                {
                    // DXT-compressed source: decode one texel
                    pctPIXEL px;
                    pctGetDXTPixel(pPict, j, i, mipMapNmb, &px, slice, face);
                    int r = px.___u1.r, g = px.___u2.g, b = px.___u3.b, a = px.a;
                    if (scaleR != 1.0f) r = (int)((float)r * scaleR);
                    if (scaleG != 1.0f) g = (int)((float)g * scaleG);
                    if (scaleB != 1.0f) b = (int)((float)b * scaleB);
                    if (scaleA != 1.0f) a = (int)((float)a * scaleA);
                    rOut = r << pDstFmt->shift[1];
                    gOut = g << pDstFmt->shift[2];
                    bOut = b << pDstFmt->shift[3];
                    aOut = a << pDstFmt->shift[0];
                }
                else if ((srcPixState & 0x20) != 0)
                {
                    // linear-addressable source: use the generic per-pixel getter
                    pctPIXEL px;
                    pctGetPixel(pPict, j, i, mipMapNmb, &px, slice, face);
                    int r = px.___u1.r, g = px.___u2.g, b = px.___u3.b, a = px.a;
                    if (scaleR != 1.0f) r = (int)((float)r * scaleR);
                    if (scaleG != 1.0f) g = (int)((float)g * scaleG);
                    if (scaleB != 1.0f) b = (int)((float)b * scaleB);
                    if (scaleA != 1.0f) a = (int)((float)a * scaleA);
                    rOut = r << pDstFmt->shift[1];
                    gOut = g << pDstFmt->shift[2];
                    bOut = b << pDstFmt->shift[3];
                    aOut = a << pDstFmt->shift[0];
                }
                else
                {
                    // raw source: read bpp bytes and unpack via mask/shift
                    int swizzleOffs = 0;
                    if ((srcPixState & 0x40) != 0)
                        swizzleOffs = pctGetSwizzleOffs(w, h, srcFormat->bitpp, j, i);
                    int bpp = srcFormat->bpp;
                    unsigned int packed = m3dMaskBuffer2DWordReverce(bpp, &srcBase[swizzleOffs]);
                    if ((srcFormat->state & 0x40) == 0)
                        srcBase += bpp;

                    int r = (srcFormat->mask[1] & packed) >> srcFormat->shift[1];
                    if (scaleR != 1.0f) r = (int)((float)r * scaleR);
                    int g = (srcFormat->mask[2] & packed) >> srcFormat->shift[2];
                    if (scaleG != 1.0f) g = (int)((float)g * scaleG);
                    int b = (srcFormat->mask[3] & packed) >> srcFormat->shift[3];
                    if (scaleB != 1.0f) b = (int)((float)b * scaleB);
                    int a = (srcFormat->mask[0] & packed) >> srcFormat->shift[0];
                    if (scaleA != 1.0f) a = (int)((float)a * scaleA);

                    rOut = r << pDstFmt->shift[1];
                    gOut = g << pDstFmt->shift[2];
                    bOut = b << pDstFmt->shift[3];
                    aOut = a << pDstFmt->shift[0];
                }

                // pack destination texel and write pDstFmt->bpp bytes (big-endian, high-to-low)
                int packedOut = aOut + bOut + gOut + rOut;
                int dstSwizzleOffs = 0;
                if ((pDstFmt->state & 0x40) != 0)
                    dstSwizzleOffs = pctGetSwizzleOffs(w, h, pDstFmt->bitpp, j, i);
                unsigned char *dstPixel = &rowPtr[dstSwizzleOffs];
                int skip = 4 - pDstFmt->bpp;

                unsigned char packedBytes[4];
                packedBytes[0] = (unsigned char)(packedOut >> 24);
                packedBytes[1] = (unsigned char)(packedOut >> 16);
                packedBytes[2] = (unsigned char)(packedOut >> 8);
                packedBytes[3] = (unsigned char)(packedOut);
                if (skip < 4)
                    _blkmov(dstPixel, &packedBytes[skip], 4 - skip);

                if ((pDstFmt->state & 0x40) == 0)
                    rowPtr += pDstFmt->bpp;
            }

            if ((pDstFmt->state & 0x40) == 0)
                rowPtr += pitch - pDstFmt->bpp * w;
        }

        if (sameFormat)
        {
            slicePtr += slicePitch;
        }
        else if (!IGNORE_STRONG_ASSERT && depth != 1)
        {
            // non-identical conversion only supports a single depth slice
            // call through a null this-pointer (canonical 4-arg member Crash); matches the decompile's
            // static-style STRONG_ASSERT_DUMMY::Crash(nullptr, ...) invocation.
            ((STRONG_ASSERT_DUMMY *)nullptr)->Crash("sz == 1",
                "D:\\Projects\\code\\common\\src.sys\\picture\\picture.cpp",
                2637, dsStrongAssertMessage);
        }
    }
}
