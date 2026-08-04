#include "headers/d3d_boundary.h"

HRESULT IDirect3DVolumeTexture9_LockBox(D3DVolumeTexture *pThis, unsigned int Level, D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, unsigned int Flags)
{
    D3DVolumeTexture_LockBox(pThis, Level, pLockedVolume, pBox, Flags);
    return 0;
}
