// _d3dXenonSet16BitFilterEnable @0x827BA220 — swap a texture's DataFormat (low 6 bits of GPU
// fetch-constant dword 1) between its 8bpp base and 16bpp filtered sibling, so the GPU samples it
// with gamma-correct 16-bit filtering when `enable` is set.
#include "headers/D3DBaseTexture.h"
#include "headers/GPUTEXTUREFORMAT.h"

extern GPUTEXTUREFORMAT _d3dXenonMake16bppFilteredFormat(GPUTEXTUREFORMAT fmt);
extern GPUTEXTUREFORMAT _d3dXenonMake8bppFilteredFormat(GPUTEXTUREFORMAT fmt);

void _d3dXenonSet16BitFilterEnable(D3DBaseTexture *lpTex, bool enable)
{
    unsigned int cur = lpTex->Format.dword[1];
    GPUTEXTUREFORMAT baseFmt = (GPUTEXTUREFORMAT)(cur & 0x3F);
    unsigned char newFmt = enable ? (unsigned char)_d3dXenonMake16bppFilteredFormat(baseFmt)
                                  : (unsigned char)_d3dXenonMake8bppFilteredFormat(baseFmt);
    lpTex->Format.dword[1] = (newFmt & 0x3F) | (cur & 0xFFFFFFC0);
}
