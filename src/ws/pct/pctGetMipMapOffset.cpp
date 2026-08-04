#include "../../headers/pctHEADER.h"
#include "../../headers/pctFORMAT.h"
#include "../../headers/pct_format.h"
#include "../../headers/pct_boundary.h"

// 0x827C1050 — byte offset (within one face) of mip level `mipMap`, i.e. the sum
// of the per-level sizes of all mips before it. The per-level size math is
// identical to pctGetMipMapMemSize (inlined here per level).
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
int pctGetMipMapOffset(pctHEADER *pHdr, int mipMap)
{
    int offset = 0;
    for (int level = 0; level < mipMap; ++level)
    {
        int depth = pHdr->sz;
        int height = pHdr->sy >> level;
        int width = pHdr->sx >> level;

        int texelCount;
        if (depth <= 1)
            texelCount = width * height;
        else
            texelCount = (depth >> level) * width * height;

        int levelSize;
        switch (pHdr->format)
        {
        case PCT_FORMAT_DXT1_OPAQUE:
        case PCT_FORMAT_DXT1_TRANSPARENT:
            if (width < 4)  width = 4;
            if (height < 4) height = 4;
            {
                int blockBytes = 4 * height * width + 7;
                if (depth <= 1)
                    levelSize = blockBytes >> 3;
                else
                    levelSize = (blockBytes >> 3) * (depth >> level);
            }
            break;

        case PCT_FORMAT_DXT2:
        case PCT_FORMAT_DXT4:
            levelSize = 4;
            break;

        case PCT_FORMAT_DXT3:
        case PCT_FORMAT_DXT5:
        case PCT_FORMAT_DXN:
            if (width < 4)  width = 4;
            if (height < 4) height = 4;
            if (depth <= 1)
                levelSize = height * width;
            else
                levelSize = (depth >> level) * height * width;
            break;

        case PCT_FORMAT_P4:
        case PCT_FORMAT_AP4:
            levelSize = texelCount % 2 + texelCount / 2;
            break;

        case PCT_FORMAT_DXT5A:
            if (width < 4)  width = 4;
            if (height < 4) height = 4;
            if (depth <= 1)
                levelSize = (height * width) >> 1;
            else
                levelSize = ((depth >> level) * height * width) >> 1;
            break;

        default:
            levelSize = pctGetFormat(pHdr->format)->bpp * texelCount;
            break;
        }
        offset += levelSize;
    }
    return offset;
}
