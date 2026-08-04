#include "headers/d3d_boundary.h"

HRESULT IDirect3DTexture9_LockRect(D3DTexture *pThis, unsigned int Level, D3DLOCKED_RECT *pLockedRect, const tagRECT *pRect, unsigned int Flags)
{
    D3DTexture_LockRect(pThis, Level, pLockedRect, pRect, Flags);
    return 0;
}
