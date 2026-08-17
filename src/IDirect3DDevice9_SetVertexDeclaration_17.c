/* IDirect3DDevice9_SetVertexDeclaration_17 @0x837A6EB0 */
#include "headers/d3d_boundary.h"
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);

HRESULT IDirect3DDevice9_SetVertexDeclaration_17(D3DDevice *pThis, D3DVertexDeclaration *pDecl)
{
    D3DDevice_SetVertexDeclaration(pThis, pDecl);
    return 0;
}
