/* IDirect3DSurface9_GetDesc_0 @0x836F34A8 */
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

HRESULT IDirect3DSurface9_GetDesc_0(D3DSurface *pThis, _D3DSURFACE_DESC *pDesc)
{
    D3DSurface_GetDesc(pThis, pDesc);
    return 0;
}
