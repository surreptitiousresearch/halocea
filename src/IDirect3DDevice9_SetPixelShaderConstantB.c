#include "headers/d3d_boundary.h"
/* D3DDevice_SetPixelShaderConstantB's declaration home is chicago_shaders.h (the only header that
 * carries it); its signature there is the DB decl verbatim. */
#include "headers/chicago_shaders.h"

HRESULT IDirect3DDevice9_SetPixelShaderConstantB(D3DDevice *pThis, unsigned int StartRegister, const int *pConstantData, unsigned int BoolCount)
{
    D3DDevice_SetPixelShaderConstantB(pThis, StartRegister, pConstantData, BoolCount);
    return 0;
}
