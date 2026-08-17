/* IDirect3DSurface9_UnlockRect @0x8369B4F8 */
#include "headers/d3d_boundary.h"
extern void D3DSurface_UnlockRect(D3DSurface *surface);

HRESULT IDirect3DSurface9_UnlockRect(D3DSurface *pThis)
{
    D3DSurface_UnlockRect(pThis);
    return 0;
}
