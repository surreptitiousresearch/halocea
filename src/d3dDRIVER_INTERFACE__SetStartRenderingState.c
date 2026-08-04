#include "headers/d3d_render_state_boundary.h"

// 0x826942CC
void d3dDRIVER_INTERFACE::SetStartRenderingState()
{
    D3DDevice_SetRenderState_ZEnable(this->pDevice, 1u);
    D3DDevice_SetRenderState_ZWriteEnable(this->pDevice, 1u);
    D3DDevice_SetRenderState_ZFunc(this->pDevice, 3u); // D3DCMP_LESSEQUAL
    D3DDevice_SetRenderState_AlphaRef(this->pDevice, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(this->pDevice, 0);
    D3DDevice_SetRenderState_AlphaFunc(this->pDevice, 6u); // D3DCMP_NOTEQUAL
    D3DDevice_SetRenderState_CullMode(this->pDevice, 0);
    D3DDevice_SetRenderState_FillMode(this->pDevice, 0);
    D3DDevice_SetRenderState_ZEnable(this->pDevice, 1u); // redundant re-set, matches decompiler verbatim
}
