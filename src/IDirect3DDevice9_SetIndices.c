/* IDirect3DDevice9_SetIndices @0x8369B348 */
#include "headers/d3d_boundary.h"
extern void D3DDevice_SetIndices(D3DDevice *pDevice, D3DIndexBuffer *pIndexData);

HRESULT IDirect3DDevice9_SetIndices(D3DDevice *pThis, D3DIndexBuffer *pIndexData)
{
    D3DDevice_SetIndices(pThis, pIndexData);
    return 0;
}
