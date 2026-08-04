#include "../../headers/d3d_driver.h"
#include "../../headers/ws/txm/txmD3D_TEX.h"
#include "../../headers/ws/vid/vidLOCK.h"
#include "../../headers/_D3DRECT.h"
#include "../../headers/_D3DFORMAT.h"
#include "../../headers/d3d_render_boundary.h" // D3DDevice_GetRenderTarget, D3DSurface

// boundary -- the video-driver critical-section singleton and the active driver instance.
extern vidLOCK vidLock;
extern vidDRIVER *vidDriver;

// boundary -- Xbox 360 D3D SDK / Xenon helpers.
extern int  _d3dXenonSurfaceGetGammaEnable(D3DSurface *pSurface);
extern int  _d3dXenonTextureGetGammaEnable(D3DBaseTexture *pTexture);
extern void D3DDevice_Resolve(D3DDevice *pDevice, unsigned int flags, const void *pRect,
                              D3DBaseTexture *pDestTexture, const void *pSourceSurface,
                              int destSubResource, int srcSubResource, const void *pClearColorRect,
                              float clearZ, unsigned int clearStencil, const void *pResolveParams);

// 0x8268E698 -- ?CopyBackBufferToTexture@d3dDRIVER@@... (virtual)  D3d_drv_8.cpp:2384
// Resolve the current color render target (back buffer) into `pTex`. Guarded by the device-ready
// state bit 0x04000000 (bit 26). Rejects the copy when the texture is missing its hardware handle,
// is itself a render target (state bit 0x800) or lacks the color-copy-capable bit (state bit 0x100).
// width/height == -1 resolves the whole texture; otherwise the (x,y,w,h) sub-rectangle. Serialized
// on the video lock. Returns 1 on success, 0 otherwise.
int d3dDRIVER::CopyBackBufferToTexture(txmD3D_TEX *pTex, int x, int y, int width, int height)
{
    if (((this->base.state.state >> 26) & 1) == 0)
        return 0;

    // Unbind any texture sources so the target can be used as a resolve destination.
    vidDriver->drvInterface->ClearSource();

    if (!pTex)
        return 0;
    if (!pTex->GetD3DTex())
        return 0;

    unsigned int state = pTex->state.state;
    if (((state >> 11) & 1) != 0 || ((state >> 8) & 1) == 0)
        return 0;

    vidLock.Lock("D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 2384);

    _D3DRECT rect;
    if (width == -1 || height == -1)
    {
        rect.x1 = 0;
        rect.y1 = 0;
        rect.x2 = pTex->GetSizeX();
        rect.y2 = pTex->GetSizeY();
    }
    else
    {
        rect.x1 = x;
        rect.y1 = y;
        rect.x2 = x + width;
        rect.y2 = y + height;
    }

    // 16-bit-per-channel targets need the EDRAM 64bpp resolve flags (0xEC000000).
    unsigned int resolveFlags = 0;
    if (pTex->format == D3DFMT_A16B16G16R16)
        resolveFlags = 0xEC000000u;

    pTex->PrepareSurfForColorResolve();

    // Gamma queries are made for their side effects on the resolve source/dest state; results unused.
    _d3dXenonSurfaceGetGammaEnable(D3DDevice_GetRenderTarget(this->pDevice, 0));
    _d3dXenonTextureGetGammaEnable(pTex->GetD3DTex());

    D3DDevice_Resolve(this->pDevice, resolveFlags, &rect, pTex->GetD3DTex(),
                      nullptr, 0, 0, nullptr, 0.0f, 0, nullptr);

    pTex->ReleaseSurfForColorResolve();
    vidLock.Unlock("D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 2384);
    return 1;
}
