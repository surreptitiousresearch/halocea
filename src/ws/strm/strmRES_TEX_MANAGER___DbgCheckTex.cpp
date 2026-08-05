#include "../../headers/ws/strm/strmRES_TEX_MANAGER.h"
#include "../../headers/ws/strm/strmVID_BUFFER.h"
#include "../../headers/ws/txm/txmD3D_TEX.h"
#include "../../headers/ws/strm/strm_boundary.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// strmRES_TEX_MANAGER::_DbgCheckTex @ 0x82670390
// Debug-only consistency check between a texture's resident mip level and which of its two
// uploaded surfaces (low-res vs hi-res) is actually current, skipped once the texture has been
// released (state.state bit 28). `pTex` is really a txmD3D_TEX* at runtime — the fields touched
// here (lpSysSurf/lpSysSurfLowRes/lpSysSurfHiRes/memHandle) live past the plain txmTEXTURE
// layout; the parameter is declared txmTEXTURE* per the mangled name and cast here, matching the
// binary's unchecked cast.
void strmRES_TEX_MANAGER::_DbgCheckTex(txmTEXTURE *pTex)
{
    if ((pTex->state.state >> 28) & 1)
        return;

    txmD3D_TEX *pD3DTex = static_cast<txmD3D_TEX *>(pTex);

    int smallerSize = pTex->GetSizeY();
    int largerSize = pTex->GetSizeX();
    if (largerSize >= smallerSize)
        largerSize = smallerSize;

    // 31 - countLeadingZeros(largerSize) == the mip index whose dimension equals largerSize.
    if (31 - __cntlzw(largerSize) == pTex->strmResource->minMip) {
        if (!IGNORE_STRONG_ASSERT && pD3DTex->lpSysSurf != pD3DTex->lpSysSurfLowRes)
            STRONG_ASSERT_DUMMY().Crash("pTex->lpSysSurf == pTex->lpSysSurfLowRes",
                "D:\\Projects\\code\\common\\src.sys\\drv\\strm_res_tex_mng.cpp", 729,
                empty_string);
    } else {
        if (!IGNORE_STRONG_ASSERT && pD3DTex->lpSysSurf != pD3DTex->lpSysSurfHiRes)
            STRONG_ASSERT_DUMMY().Crash("pTex->lpSysSurf == pTex->lpSysSurfHiRes",
                "D:\\Projects\\code\\common\\src.sys\\drv\\strm_res_tex_mng.cpp", 731,
                empty_string);

        unsigned int baseSize = 0, mipSize = 0;
        XGGetTextureLayout(pD3DTex->lpSysSurf, nullptr, &baseSize, nullptr, nullptr, 0x1000u,
                            nullptr, &mipSize);

        if (!IGNORE_STRONG_ASSERT && pD3DTex->memHandle->offset != (unsigned char *)baseSize)
            STRONG_ASSERT_DUMMY().Crash("pTex->memHandle->offset == (BYTE*)base",
                "D:\\Projects\\code\\common\\src.sys\\drv\\strm_res_tex_mng.cpp", 735,
                empty_string);
    }

    prev = *static_cast<strmRESOURCE_TEXTURE_XENON *>(pTex->strmResource);
}
