#include "headers/d3d_boundary.h"

HRESULT IDirect3DVolumeTexture9_UnlockBox_0(D3DVolumeTexture *pThis, unsigned int Level)
{
    D3DVolumeTexture_UnlockBox(pThis, Level);
    return 0;
}
