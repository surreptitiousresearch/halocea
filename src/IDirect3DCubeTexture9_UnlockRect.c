/* IDirect3DCubeTexture9_UnlockRect @0x8369B478 */
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

HRESULT IDirect3DCubeTexture9_UnlockRect(D3DCubeTexture *pThis, _D3DCUBEMAP_FACES FaceType, unsigned int Level)
{
    D3DCubeTexture_UnlockRect(pThis, FaceType, Level);
    return 0;
}
