#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/rend/rendV3D.h"
#include "../../headers/ws/rend/PASS_DESC_BASE.h"
#include "../../headers/ws/txm/txmTEXTURE.h"

// rendDRIVER::RenderPoly(int, rendV3D*, BLENDMODE, unsigned long, txmTEXTURE*) @ 0x827C9D80
// ?RenderPoly@rendDRIVER@@QAAXHPAUrendV3D@@W4BLENDMODE@PASS_DESC_BASE@@KPAVtxmTEXTURE@@@Z
//
// Draw a screen-space, color-modulated, optionally-textured convex polygon (`nVert` rendV3D
// vertices) as a single triangle fan. Builds a PASS_DESC_BASE with the constant modulate color
// unpacked from `color` (0xAARRGGBB → colorConst.{r,g,b,a} in 0..1), no back-face cull and no
// depth write/test, configures the driver for a screen-space color pass, clears the transform,
// and submits the vertices through DynGeomDraw.
//
// ABI note (RenderPoly is a known decompiler offender): the disasm shows the true parameters are
// (this, nVert, vlist, blendMode, color, texture) — matching the mangled name. The decompiler
// merged color+texture into a single __int64 register pair; corrected here.
// The texture handle behaves as the descriptor's txmTEXTURE_PTR: its refcount is bumped for the
// duration of the draw and released afterward (only while the texture manager is alive).

struct txmMANAGER;   // txm boundary (opaque here)
extern rendDRIVER *rendDrv;
extern txmMANAGER *txmManager;
extern int osLockedIncrement(int *value);

static const float kByteToUnit = 0.0039215689f; // 1/255

void rendDRIVER::RenderPoly(int nVert, rendV3D *vlist, PASS_DESC_BASE::BLENDMODE blendMode,
                            unsigned int color, txmTEXTURE *texture)
{
    PASS_DESC_BASE desc;
    desc.rgbSrc         = (PASS_DESC_BASE::RGBSRC)0;
    desc.alphaSrc       = (PASS_DESC_BASE::ALPHASRC)0;
    desc.colorConst.r   = (float)((color >> 16) & 0xFF) * kByteToUnit;
    desc.colorConst.g   = (float)((color >> 8) & 0xFF) * kByteToUnit;
    desc.colorConst.b   = (float)(color & 0xFF) * kByteToUnit;
    desc.colorConst.a   = (float)((color >> 24) & 0xFF) * kByteToUnit;
    desc.blendMode      = blendMode;
    desc.blendModeAlpha = (PASS_DESC_BASE::BLENDMODEALPHA)0;
    desc.zWrite         = false;
    desc.zTest          = false;
    desc.noCull         = true;
    desc.outputHDR      = false;
    desc.disableGamma   = false;

    // desc.tex (txmTEXTURE_PTR) adopts `texture`, bumping its refcount.
    desc.tex.ptr = texture;
    if (texture)
        osLockedIncrement(&texture->refCount);

    rendDrv->Configure(0, &desc, 0x40000001, 0x10u,
                       2 /* COORDSPACE_SCREEN */, 0 /* RENDERBLOCK_COLOR */, 0, 0);
    SetTransform(nullptr);

    // fvf = 0x0000001040000001, callerID = 'POLY' (0x594C4F50); no index buffer (trifan).
    DynGeomDraw(REND_PRIMTYPE_TRIFAN, vlist, nVert, nullptr, 0,
                0x0000001040000001ull, 0x594C4F50u);

    if (desc.tex.ptr && txmManager)
        desc.tex.ptr->Release();
}
