#include "../../headers/d3dDRIVER_INTERFACE.h"
#include "../../headers/ws/vid/vidFVF_DESCR.h"

// boundary -- Xbox 360 D3D SDK: set the active vertex declaration.
extern "C" void D3DDevice_SetVertexDeclaration(D3DDevice *pDevice, D3DVertexDeclaration *pDecl);

// 0x82694898 -- ?EnsureShaderDecl@d3dDRIVER_INTERFACE@@UAAXPAVvidFVF_DESCR@@HH@Z (virtual)
// Cache the requested FVF descriptor and apply its owned D3D vertex declaration. In the d3d build
// the D3DVertexDeclaration* is stored immediately past the vidFVF_DESCR object, at offset 0x94
// (the decompiler renders it as `pDescr[1].__vftable`). A null descriptor clears the declaration.
// The freqVert/freqFace stream-frequency parameters are part of the vtable signature but are not
// referenced by this build's body.
void d3dDRIVER_INTERFACE::EnsureShaderDecl(vidFVF_DESCR *pDescr, int /*freqVert*/, int /*freqFace*/)
{
    this->pFVFDescr = pDescr;

    if (pDescr)
    {
        // Declaration pointer lives just past the descriptor (offset 0x94 == sizeof(vidFVF_DESCR)).
        D3DVertexDeclaration *pDecl =
            *reinterpret_cast<D3DVertexDeclaration *const *>(reinterpret_cast<const char *>(pDescr) + 0x94);
        D3DDevice_SetVertexDeclaration(this->pDevice, pDecl);
    }
    else
    {
        D3DDevice_SetVertexDeclaration(this->pDevice, nullptr);
    }
}
