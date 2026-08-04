/* hcex_tex_get_hardware_format @0x823F2100 — ensure a ws-engine texture is realized on the GPU, returning
 * whether it currently has a hardware surface.
 *
 *   - hcex_tex_block forces the blocking-stream path on.
 *   - load==0: just report whether the texture already has a D3D surface (no side effects).
 *   - Otherwise: optionally record the texture name in the debug "used textures" list, force it resident via
 *     the texture manager (strong-assert on failure) and mark it used this frame. If it now has a hardware
 *     surface, return 1. If not, seed the streaming accumulator (bump splitCount, set the mip/dist coefs) so
 *     the streamer prioritises it; when blocking, wrap a NecessaryStreamTexture kick in a PIX event and
 *     return 1. Return 0 when still not resident and non-blocking.
 *
 * txm* / strm* / D3D / PIX are ws-engine + D3D boundaries (declared, not reversed); struct offsets are
 * DB-verified (txmTEXTURE.name@4 / .strmResource@144, strmRESOURCE_TEXTURE.accDataVector@104,
 * ACCUM_DATA.lastData@192 with mipCoef/coefDist/dist2 as m2dV and splitCount@0x18). */

#include "../headers/hcex/hcex_tex_hw_format_boundary.h"

extern "C" int hcex_tex_get_hardware_format(txmD3D_TEX *tex, int block, int load)
{
    if ( hcex_tex_block )
        block = 1;

    if ( !load )
        return txmD3D_TEX__GetD3DTex(tex) != 0;

    if ( dbg_hcexDumpUsedTexOn.value )
    {
        dsCMP cmp = {}; // dsCMP is a stateless struct (canonical), not an int
        dsVECTOR_dsTSTRING_InsertSorted(&hcexTexList, &tex->name, &cmp, INS_DUP_IGNORE);
    }

    int loaded = txmMANAGER_Load(txmManager, tex, 1);
    if ( !IGNORE_STRONG_ASSERT && !loaded )
        STRONG_ASSERT_DUMMY_Crash(0, "rc", "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 720,
                                 empty_string);

    D3DBaseTexture *hw = txmD3D_TEX__GetD3DTex(tex);
    txmTEXTURE__MarkAsUsedInRender(tex);
    if ( hw )
        return 1;

    /* No hardware surface yet — bias the streaming accumulator toward this texture. */
    strmRESOURCE_TEXTURE *res = tex->strmResource;
    res->accDataVector.lastData.mipCoef.y  = 0.0f;
    res->accDataVector.lastData.mipCoef.x  = 0.0f;
    res->accDataVector.lastData.coefDist.y = 0.1f;
    res->accDataVector.lastData.splitCount = res->accDataVector.lastData.splitCount + 1;
    res->accDataVector.lastData.coefDist.x = 0.1f;
    res->accDataVector.lastData.dist2.y    = 0.0f;
    res->accDataVector.lastData.dist2.x    = 0.0f;

    if ( block )
    {
        char label[176];
        _snprintf_0(label, 0x7F, "block '%s'", tex->name.pBuffer->str);
        label[127] = 0;
        osPIXBeginEvent(label);
        strmRES_TEX_MANAGER_NecessaryStreamTexture(&strmRES_TEX_MANAGER_mng, tex);
        osPIXEndEvent();
        return 1;
    }

    return 0;
}
