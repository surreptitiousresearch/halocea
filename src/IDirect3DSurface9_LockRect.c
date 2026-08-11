#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

HRESULT IDirect3DSurface9_LockRect(D3DSurface *pThis, _D3DLOCKED_RECT *pLockedRect, const tagRECT *pRect, unsigned int Flags)
{
    D3DSurface_LockRect(pThis, pLockedRect, pRect, Flags);
    return 0;
}
