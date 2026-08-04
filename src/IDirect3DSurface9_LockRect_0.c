#include "headers/d3d_boundary.h"

HRESULT IDirect3DSurface9_LockRect_0(D3DSurface *pThis, D3DLOCKED_RECT *pLockedRect, const tagRECT *pRect, unsigned int Flags)
{
    D3DSurface_LockRect(pThis, pLockedRect, pRect, Flags);
    return 0;
}
