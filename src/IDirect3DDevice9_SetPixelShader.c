/* IDirect3DDevice9_SetPixelShader @0x8369B370 */
#include "headers/d3d_boundary.h"
extern void D3DDevice_SetPixelShader(D3DDevice *device, D3DPixelShader *shader);

HRESULT IDirect3DDevice9_SetPixelShader(D3DDevice *pThis, D3DPixelShader *pShader)
{
    D3DDevice_SetPixelShader(pThis, pShader);
    return 0;
}
