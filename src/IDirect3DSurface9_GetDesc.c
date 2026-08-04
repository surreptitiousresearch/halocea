#include "headers/d3d_boundary.h"

HRESULT IDirect3DSurface9_GetDesc(D3DSurface *pThis, D3DSURFACE_DESC *pDesc)
{
    D3DSurface_GetDesc(pThis, pDesc);
    return 0;
}
