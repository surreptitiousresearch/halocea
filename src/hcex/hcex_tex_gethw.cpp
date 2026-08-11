/* hcex_tex_gethw @0x823F0560 — resolve a txm texture wrapper to its hardware D3D texture, forcing it
 * resident first. Loads (streams in) the texture via the texture manager, then returns its D3DTexture; if
 * the texture has no hardware surface it falls back to the manager's white texture. Strong-asserts if the
 * load fails or the white-texture fallback is itself missing. `name` is a debug label, unused at runtime.
 * txmMANAGER / txmD3D_TEX / D3D are ws-engine + D3D boundaries (declared, not reversed). */

#include "../headers/txm_boundary.h"    /* txmMANAGER, txmManager, txmMANAGER_white_tex */
#include "../headers/txm_d3d_tex.h"     /* txmD3D_TEX, txmD3D_TEX__GetD3DTex, D3DTexture */

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h" // the only Crash is a C++ member, not a C symbol
extern int  txmMANAGER_Load(txmMANAGER *self, txmD3D_TEX *tex, int force_resident);

extern "C" D3DTexture *hcex_tex_gethw(txmD3D_TEX *tex, const char *name)
{
    int loaded = txmMANAGER_Load(txmManager, tex, 1);
    if ( !IGNORE_STRONG_ASSERT && !loaded )
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("rc", "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 761, empty_string);

    D3DTexture *hw = txmD3D_TEX__GetD3DTex(tex);
    if ( !hw )
    {
        /* whiteTex.ptr is stored as a txmTEXTURE* but is really a txmD3D_TEX* wrapper here */
        txmD3D_TEX *white = (txmD3D_TEX *)txmMANAGER_white_tex(txmManager)->ptr;
        if ( !IGNORE_STRONG_ASSERT && (!white || !txmD3D_TEX__GetD3DTex(white)) )
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("pTex && pTex->GetD3DTex()",
                "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 769, empty_string);
        return txmD3D_TEX__GetD3DTex(white);
    }
    return hw;
}
