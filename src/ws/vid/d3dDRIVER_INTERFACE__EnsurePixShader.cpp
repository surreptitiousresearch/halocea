/* ?EnsurePixShader@d3dDRIVER_INTERFACE@@UAA_NPAVvidSHADER@@_N@Z @0x82694910 */
#include "../../headers/d3dDRIVER_INTERFACE.h"

// boundary -- Xbox 360 D3D SDK: set the active pixel shader.
extern "C" void D3DDevice_SetPixelShader(D3DDevice *pDevice, D3DPixelShader *pShader);

// 0x82694910 -- ?EnsurePixShader@d3dDRIVER_INTERFACE@@UAA_NPAVD3DPixelShader@@_N@Z (virtual)
// Redundancy-filtered pixel-shader bind. `isEmptyShader` selects the null shader: the disasm
// computes it branchlessly as (isEmptyShader - 1) & pShaderPix -- when isEmptyShader is true the
// mask is 0 (bind null), otherwise the mask is all-ones (bind pShaderPix). No-op (returns false)
// when the resolved shader is already active; otherwise sets it, caches it, and returns true.
bool d3dDRIVER_INTERFACE::EnsurePixShader(D3DPixelShader *pShaderPix, bool isEmptyShader)
{
    D3DPixelShader *resolved = isEmptyShader ? nullptr : pShaderPix;

    if (this->pShaderPix == resolved)
        return false;

    D3DDevice_SetPixelShader(this->pDevice, resolved);
    this->pShaderPix = resolved;
    return true;
}
