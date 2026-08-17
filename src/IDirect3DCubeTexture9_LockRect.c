/* IDirect3DCubeTexture9_LockRect @0x8369B450 */
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

HRESULT IDirect3DCubeTexture9_LockRect(D3DCubeTexture *pThis, _D3DCUBEMAP_FACES FaceType, unsigned int Level, _D3DLOCKED_RECT *pLockedRect, const tagRECT *pRect, unsigned int Flags)
{
    D3DCubeTexture_LockRect(pThis, FaceType, Level, pLockedRect, pRect, Flags);
    return 0;
}
