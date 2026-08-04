#include "headers/d3d_boundary.h"

HRESULT IDirect3DCubeTexture9_UnlockRect(D3DCubeTexture *pThis, D3DCUBEMAP_FACES FaceType, unsigned int Level)
{
    D3DCubeTexture_UnlockRect(pThis, FaceType, Level);
    return 0;
}
