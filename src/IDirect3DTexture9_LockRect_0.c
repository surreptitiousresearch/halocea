#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

HRESULT IDirect3DTexture9_LockRect_0(D3DTexture *pThis, unsigned int Level, _D3DLOCKED_RECT *pLockedRect, const tagRECT *pRect, unsigned int Flags)
{
    D3DTexture_LockRect(pThis, Level, pLockedRect, pRect, Flags);
    return 0;
}
