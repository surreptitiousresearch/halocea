#include "headers/d3d_boundary.h"

HRESULT IDirect3DDevice9_SetPixelShaderConstantB(D3DDevice *pThis, unsigned int StartRegister, const int *pConstantData, unsigned int BoolCount)
{
    D3DDevice_SetPixelShaderConstantB(pThis, StartRegister, pConstantData, BoolCount);
    return 0;
}
