/* IDirect3DDevice9_SetVertexShader_21 @0x837A9F58 */
#include "headers/d3d_boundary.h"
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);

HRESULT IDirect3DDevice9_SetVertexShader_21(D3DDevice *pThis, D3DVertexShader *pShader)
{
    D3DDevice_SetVertexShader(pThis, pShader);
    return 0;
}
