/* HCEXLoadingScreenDestroyResources @0x823F37E8 — release the four D3D resources the HCEX
 * loading screen owns (vertex declaration, vertex/pixel shaders, background/mask textures),
 * nulling each pointer after release. */

#include "../headers/d3d_render_boundary.h"

extern "C" D3DVertexShader      *spHCEXLoadingScreenVS;
extern "C" D3DPixelShader       *spHCEXLoadingScreenPS;
extern D3DVertexDeclaration *spHCEXLoadingScreenVertexDecl;
extern "C" D3DTexture           *spHCEXLoadingScreenBackgroundTex;
extern "C" D3DTexture           *spHCEXLoadingScreenMaskTex;

void HCEXLoadingScreenDestroyResources(void)
{
    if (spHCEXLoadingScreenVertexDecl)
    {
        D3DResource_Release((D3DResource *)spHCEXLoadingScreenVertexDecl);
        spHCEXLoadingScreenVertexDecl = 0;
    }
    if (spHCEXLoadingScreenVS)
    {
        D3DResource_Release((D3DResource *)spHCEXLoadingScreenVS);
        spHCEXLoadingScreenVS = 0;
    }
    if (spHCEXLoadingScreenPS)
    {
        D3DResource_Release((D3DResource *)spHCEXLoadingScreenPS);
        spHCEXLoadingScreenPS = 0;
    }
    if (spHCEXLoadingScreenBackgroundTex)
    {
        D3DResource_Release((D3DResource *)spHCEXLoadingScreenBackgroundTex);
        spHCEXLoadingScreenBackgroundTex = 0;
    }
    if (spHCEXLoadingScreenMaskTex)
    {
        D3DResource_Release((D3DResource *)spHCEXLoadingScreenMaskTex);
        spHCEXLoadingScreenMaskTex = 0;
    }
}
