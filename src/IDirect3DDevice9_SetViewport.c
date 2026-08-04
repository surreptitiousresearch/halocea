#include "headers/d3d_boundary.h"

HRESULT IDirect3DDevice9_SetViewport(D3DDevice *pThis, const D3DVIEWPORT9 *pViewport)
{
    D3DDevice_SetViewport(pThis, pViewport);
    return 0;
}
