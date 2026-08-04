#include "../../headers/ws/txm/txmD3D_TEX.h"
#include "../../headers/ws/txm/txmMANAGER.h"
#include "../../headers/ws/txm/txm_vid_boundary.h"

/* 0x827BB8E8 -- txmD3D_TEX::GetD3DTex() const
 *
 * Split-screen viewports can each render to their own copy of a render-target texture
 * (txmTEXTURE::pSplitScreenTex[0]/[1] chain the per-viewport siblings). While a split-screen
 * transition is active (txmManager->state bit 15) and the video driver is mid-resolve/blit for
 * the current mode (vidDriver->pMode->vmState bit 10 or 11 -- mask 0xC00), this walks the
 * sibling chain -- picking slot 1 or slot 0 per txmManager->state bit 16 -- to find the texture
 * object actually backing the current viewport, then returns its resolved D3D surface handle. */
D3DBaseTexture *txmD3D_TEX::GetD3DTex() const
{
    txmD3D_TEX *tex = const_cast<txmD3D_TEX *>(this);
    txmTEXTURE *splitScreenTex0 = tex->pSplitScreenTex[0];

    if ( splitScreenTex0 )
    {
        unsigned int managerState = txmManager->state.state;

        do
        {
            if ( ((managerState >> 15) & 1) == 0 || (vidDriver->pMode->vmState & 0xC00) == 0 )
                break;

            // Deviation note: decompiler rendered this as a raw C-style cast; the sibling slots
            // are always txmD3D_TEX instances stored via the base-class txmTEXTURE* array, so a
            // static_cast downcast is used here instead.
            tex = (managerState & 0x10000) != 0
                ? static_cast<txmD3D_TEX *>(tex->pSplitScreenTex[1])
                : static_cast<txmD3D_TEX *>(splitScreenTex0);
            splitScreenTex0 = tex->pSplitScreenTex[0];
        }
        while ( splitScreenTex0 );
    }

    return tex->lpSysSurf;
}
