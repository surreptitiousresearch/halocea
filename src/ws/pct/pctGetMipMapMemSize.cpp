#include "../../headers/pctHEADER.h"
#include "../../headers/pctFORMAT.h"
#include "../../headers/pct_format.h"
#include "../../headers/pct_boundary.h"

// 0x827C0DF8 — byte size of a single mip level (one face) for the picture
// described by pHdr. Block-compressed (DXTn) formats are computed from their
// 4x4-block granularity; all other formats fall back to bpp * texel count.
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
int pctGetMipMapMemSize(pctHEADER *pHdr, char mipMap)
{
    int depth = pHdr->sz;
    int height = pHdr->sy >> mipMap;
    int width = pHdr->sx >> mipMap;

    int texelCount;
    if (depth <= 1)
        texelCount = width * height;
    else
        texelCount = (depth >> mipMap) * width * height;

    int result;
    switch (pHdr->format)
    {
    case PCT_FORMAT_DXT1_OPAQUE:
    case PCT_FORMAT_DXT1_TRANSPARENT:
        if (width < 4)  width = 4;
        if (height < 4) height = 4;
        {
            int blockBytes = 4 * height * width + 7;
            if (depth <= 1)
                result = blockBytes >> 3;
            else
                result = (blockBytes >> 3) * (depth >> mipMap);
        }
        break;

    case PCT_FORMAT_DXT2:
    case PCT_FORMAT_DXT4:
        result = 4;
        break;

    case PCT_FORMAT_DXT3:
    case PCT_FORMAT_DXT5:
    case PCT_FORMAT_DXN:
        if (width < 4)  width = 4;
        if (height < 4) height = 4;
        if (depth <= 1)
            result = height * width;
        else
            result = (depth >> mipMap) * height * width;
        break;

    case PCT_FORMAT_P4:
    case PCT_FORMAT_AP4:
        result = texelCount % 2 + texelCount / 2;
        break;

    case PCT_FORMAT_DXT5A:
        if (width < 4)  width = 4;
        if (height < 4) height = 4;
        if (depth <= 1)
            result = (height * width) >> 1;
        else
            result = ((depth >> mipMap) * height * width) >> 1;
        break;

    default:
        result = pctGetFormat(pHdr->format)->bpp * texelCount;
        break;
    }
    return result;
}
