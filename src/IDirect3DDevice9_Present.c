/* IDirect3DDevice9_Present @0x8369B088 */
#include "headers/d3d_boundary.h"
extern void D3DDevice_Present(D3DDevice *device);

HRESULT IDirect3DDevice9_Present(D3DDevice *pThis, const tagRECT *pUnusedSourceRect, const tagRECT *pUnusedDestRect, void *hUnusedDestWindowOverride, void *pUnusedDirtyRegion)
{
    D3DDevice_Present(pThis);
    return 0;
}
