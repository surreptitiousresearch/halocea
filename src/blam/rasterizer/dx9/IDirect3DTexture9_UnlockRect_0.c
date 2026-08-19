/* IDirect3DTexture9_UnlockRect_0 @0x83793380 */
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

HRESULT IDirect3DTexture9_UnlockRect_0(D3DTexture *pThis, unsigned int Level)
{
    D3DTexture_UnlockRect(pThis, Level);
    return 0;
}
