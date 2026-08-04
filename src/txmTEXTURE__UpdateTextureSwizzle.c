#include "headers/ws/txm/txmTEXTURE.h"
#include "headers/ws/txm/txm_vid_boundary.h"

// 0x826F57A4 -- derives this->swizzle (a packed 4x3-bit channel-source-select word: R|G|B|A each
// 0-7) from the four "extra map" usage bits (0x200/0x400/0x800/0x1000 -- e.g. secondary
// normal/spec/emissive maps folded into one hardware texture's RGBA channels), then propagates
// the same 4 sources to both split-screen sub-textures via SetSwizzle. No-op unless at least one
// of those bits is set.
//
// DEVIATION: the decompiler produced this as raw HIDWORD/table-index pointer arithmetic over a
// 4-int stack array ([0,1,2,3] with element 0 patched to a DXN-dependent value). Reconstructed
// here as an explicit `channelOrder[4]` array indexed the same way; behavior is preserved
// verbatim -- only the spelling is cleaned up. `5` (the pre-fill default channel source when a
// map-bit isn't set) and `channelOrder`'s [1,2,3] entries are TXM_TEX_SWIZZLE_SRC channel-index
// tokens whose symbolic names were not resolved in this batch.
void txmTEXTURE::UpdateTextureSwizzle()
{
    unsigned int maps = this->desc.usage.maps;
    if ((maps & 0x1E00) == 0)
        return;

    bool hasMap0x200  = (maps & 0x200)  != 0;
    bool hasMap0x400  = (maps & 0x400)  != 0;
    bool hasMap0x800  = (maps & 0x800)  != 0;
    bool hasMap0x1000 = (maps & 0x1000) != 0;

    int setCount = hasMap0x200 ? 1 : 0;
    if (hasMap0x400)  ++setCount;
    if (hasMap0x800)  ++setCount;
    if (hasMap0x1000) ++setCount;

    // channelOrder[0] starts as 0 but is patched per setCount/dxn support below; [1,2,3] stay 1,2,3.
    int channelOrder[4] = { 0, 1, 2, 3 };
    int firstChannel;

    if (setCount == 1)
    {
        firstChannel = vidDriver->sdrPlatform.dxn ? 0 : 1;
    }
    else if (setCount == 2)
    {
        if (vidDriver->sdrPlatform.dxn)
        {
            firstChannel = 1;
        }
        else
        {
            firstChannel = 3;
            channelOrder[1] = 1; // matches decompiler's v19=1 override on this path
        }
    }
    else
    {
        firstChannel = 1;
    }
    channelOrder[0] = firstChannel;

    TXM_TEX_SWIZZLE_SRC r = TXM_TEX_SWIZZLE_SRC_1, g = TXM_TEX_SWIZZLE_SRC_1,
                        b = TXM_TEX_SWIZZLE_SRC_1, a = TXM_TEX_SWIZZLE_SRC_1;
    int cursor = 0;

    if (hasMap0x200)
    {
        r = (TXM_TEX_SWIZZLE_SRC)firstChannel;
        cursor = 1;
    }
    if (hasMap0x400)
    {
        g = (TXM_TEX_SWIZZLE_SRC)channelOrder[cursor];
        ++cursor;
    }
    if (hasMap0x800)
    {
        b = (TXM_TEX_SWIZZLE_SRC)channelOrder[cursor];
        ++cursor;
    }
    if (hasMap0x1000)
    {
        a = (TXM_TEX_SWIZZLE_SRC)channelOrder[cursor];
    }

    this->swizzle = (unsigned short)((((r * 8 | g) * 8) | b) * 8 | a);

    txmTEXTURE *splitTex0 = this->pSplitScreenTex[0];
    if (splitTex0)
        splitTex0->SetSwizzle(r, g, b, a);

    txmTEXTURE *splitTex1 = this->pSplitScreenTex[1];
    if (splitTex1)
        splitTex1->SetSwizzle(r, g, b, a);
}
