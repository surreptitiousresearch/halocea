#include "headers/d3d_boundary.h"
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);

HRESULT IDirect3DDevice9_SetVertexShader_23(D3DDevice *pThis, D3DVertexShader *pShader)
{
    D3DDevice_SetVertexShader(pThis, pShader);
    return 0;
}
