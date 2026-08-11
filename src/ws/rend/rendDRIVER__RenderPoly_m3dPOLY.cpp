#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/rend/rendV3D.h"
#include "../../headers/ws/rend/PASS_DESC_BASE.h"
#include "../../headers/ws/m3d/m3dPOLY.h"
#include "../../headers/ws/txm/txmTEXTURE.h"

// rendDRIVER::RenderPoly(m3dPOLY*, BLENDMODE, unsigned long, txmTEXTURE*) @ 0x827C9F30
// ?RenderPoly@rendDRIVER@@QAAXPAVm3dPOLY@@W4BLENDMODE@PASS_DESC_BASE@@KPAVtxmTEXTURE@@@Z
//
// As the rendV3D overload, but the source geometry is an m3dPOLY. Each polygon vertex is copied
// into a stack rendV3D buffer (position from poly->vert[i], the constant modulate color, zeroed
// texcoords) and the convex polygon is drawn as a single triangle fan.
//
// Deviation: the shipping body hand-unrolls the vertex fill four vertices per iteration (plus a
// remainder loop) and walks the buffer via a pointer biased +0xC into each 24-byte rendV3D
// (color field first in the store order, position written at the -0xC slack). The net mapping is
// a straight per-vertex copy out[i] = { poly->vert[i], color, tc=(0,0) }; reconstructed here in
// that reduced form (verified against the disasm at 0x827C9F60-0x827CA088). The color-unpack,
// PASS_DESC_BASE setup, Configure, SetTransform and DynGeomDraw are identical to the rendV3D
// overload.

struct txmMANAGER;   // txm boundary (opaque here)
extern rendDRIVER *rendDrv;
extern txmMANAGER *txmManager;
extern int osLockedIncrement(int *value);

static const float kByteToUnit = 0.0039215689f; // 1/255

void rendDRIVER::RenderPoly(m3dPOLY *poly, PASS_DESC_BASE::BLENDMODE blendMode,
                            unsigned int color, txmTEXTURE *texture)
{
    int nVert = poly->nVert;

    rendV3D verts[20]; // m3dPOLY holds at most 20 vertices
    for (int i = 0; i < nVert; ++i) {
        verts[i].v     = poly->vert[i];
        verts[i].color = color;
        verts[i].tc.s  = 0.0f;
        verts[i].tc.t  = 0.0f;
    }

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

    desc.tex.ptr = texture;
    if (texture)
        osLockedIncrement(&texture->refCount);

    rendDrv->Configure(0, &desc, 0x40000001, 0x10u,
                       2 /* COORDSPACE_SCREEN */, 0 /* RENDERBLOCK_COLOR */, 0, 0);
    SetTransform(nullptr);

    DynGeomDraw(REND_PRIMTYPE_TRIFAN, verts, nVert, nullptr, 0,
                0x0000001040000001ull, 0x594C4F50u);

    if (desc.tex.ptr && txmManager)
        desc.tex.ptr->Release();
}
