/* SetTextureSmart @ 0x83790298 — bind a texture to a sampler stage via
 * D3DDevice_SetTexture on the global device. The 4th argument is the stage's
 * fetch-constant mask, 0x8000000000000000 >> (Stage + 32) — i.e. bit (31 - Stage). */

#include "headers/D3DDevice.h"
#include "headers/D3DBaseTexture.h"

extern D3DDevice *global_d3d_device;
extern void D3DDevice_SetTexture(D3DDevice *pDevice, unsigned int Stage,
                                 D3DBaseTexture *pTexture, unsigned __int64 mask);

void SetTextureSmart(unsigned int Stage, D3DBaseTexture *pTexture)
{
    D3DDevice_SetTexture(global_d3d_device, Stage, pTexture,
                         0x8000000000000000ULL >> (Stage + 32));
}
