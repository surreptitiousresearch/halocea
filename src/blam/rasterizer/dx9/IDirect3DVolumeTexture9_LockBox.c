/* IDirect3DVolumeTexture9_LockBox @0x8369B3F8 */
#include "headers/d3d_boundary.h"
#include "headers/d3d_render_boundary.h"

HRESULT IDirect3DVolumeTexture9_LockBox(D3DVolumeTexture *pThis, unsigned int Level, _D3DLOCKED_BOX *pLockedVolume, const _D3DBOX *pBox, unsigned int Flags)
{
    D3DVolumeTexture_LockBox(pThis, Level, pLockedVolume, pBox, Flags);
    return 0;
}
