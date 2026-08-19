#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

/* DB decl @ 0x836F3298: HRESULT __fastcall IDirect3DDevice9_SetViewport(D3DDevice *,
 * const _D3DVIEWPORT9 *). The const was dropped here before #140 only because the callee
 * declaration could not carry it; that block is gone, so the DB spelling is restored. */
HRESULT IDirect3DDevice9_SetViewport(D3DDevice *pThis, const _D3DVIEWPORT9 *pViewport)
{
    D3DDevice_SetViewport(pThis, pViewport);
    return 0;
}
