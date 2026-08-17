/* IDirect3DTexture9_UnlockRect @0x8369B3C8 */
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

HRESULT IDirect3DTexture9_UnlockRect(D3DTexture *pThis, unsigned int Level)
{
    D3DTexture_UnlockRect(pThis, Level);
    return 0;
}
