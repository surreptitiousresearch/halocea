/* IDirect3DTexture9_LockRect @0x8369B3A0 */
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

HRESULT IDirect3DTexture9_LockRect(D3DTexture *pThis, unsigned int Level, _D3DLOCKED_RECT *pLockedRect, const tagRECT *pRect, unsigned int Flags)
{
    D3DTexture_LockRect(pThis, Level, pLockedRect, pRect, Flags);
    return 0;
}
