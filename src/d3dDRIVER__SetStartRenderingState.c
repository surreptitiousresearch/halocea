#include "headers/d3d_render_state_boundary.h"

// 0x82695BBC -- re-applies the shared baseline state via drvInterface, then this driver's own
// copy of the same D3D calls, then feeds both the vertex and pixel shader constant banks a
// [1/w, 1/h, 0.5/w, 0.5/h] vector (register 5 for VS, register 24 for PS) derived from the
// current display mode -- the standard "half-pixel-offset texel-to-screen" constant pair.
void d3dDRIVER::SetStartRenderingState()
{
    this->base.drvInterface->SetStartRenderingState();

    D3DDevice_SetRenderState_ZEnable(this->pDevice, 1u);
    D3DDevice_SetRenderState_ZWriteEnable(this->pDevice, 1u);
    D3DDevice_SetRenderState_ZFunc(this->pDevice, 3u);
    D3DDevice_SetRenderState_AlphaRef(this->pDevice, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(this->pDevice, 0);
    D3DDevice_SetRenderState_AlphaFunc(this->pDevice, 6u);
    D3DDevice_SetRenderState_CullMode(this->pDevice, 0);
    D3DDevice_SetRenderState_FillMode(this->pDevice, 0);
    D3DDevice_SetRenderState_ZEnable(this->pDevice, 1u);

    float texelConst[4];
    texelConst[0] = 1.0f / (float)this->base.pMode->sx;
    texelConst[1] = 1.0f / (float)this->base.pMode->sy;
    texelConst[2] = 0.5f / (float)this->base.pMode->sx;
    texelConst[3] = 0.5f / (float)this->base.pMode->sy;
    vidDriver->base.drvInterface->EnsureVertShaderConst(5, texelConst, 1);

    float texelConst2[4];
    texelConst2[0] = 1.0f / (float)this->base.pMode->sx;
    texelConst2[1] = 1.0f / (float)this->base.pMode->sy;
    texelConst2[2] = 0.5f / (float)this->base.pMode->sx;
    texelConst2[3] = 0.5f / (float)this->base.pMode->sy;
    vidDriver->base.drvInterface->EnsurePixShaderConst(24, texelConst2, 1);
}
