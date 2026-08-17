/* ?EnsureVertShader@d3dDRIVER_INTERFACE@@UAA_NPAVvidSHADER@@@Z @0x826948B8 */
#include "../../headers/d3dDRIVER_INTERFACE.h"

// boundary -- Xbox 360 D3D SDK: set the active vertex shader.
extern "C" void D3DDevice_SetVertexShader(D3DDevice *pDevice, D3DVertexShader *pShader);

// 0x826948B8 -- ?EnsureVertShader@d3dDRIVER_INTERFACE@@UAA_NPAVD3DVertexShader@@@Z (virtual)
// Redundancy-filtered vertex-shader bind: no-op (returns false) when the requested shader is
// already active; otherwise sets it on the device, caches it, and returns true.
bool d3dDRIVER_INTERFACE::EnsureVertShader(D3DVertexShader *pShaderVert)
{
    if (this->pShaderVert == pShaderVert)
        return false;

    D3DDevice_SetVertexShader(this->pDevice, pShaderVert);
    this->pShaderVert = pShaderVert;
    return true;
}
