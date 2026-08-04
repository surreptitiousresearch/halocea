// _d3dXenonTextureSetGammaEnable @0x827BA070 — toggle the sign/gamma bits (0xFC: SignX/Y/Z/W and
// clamp low bits) in dword 0 of a texture's GPU fetch constant. enable => set them, else clear.
#include "headers/D3DBaseTexture.h"

void _d3dXenonTextureSetGammaEnable(D3DBaseTexture *lpTex, bool enable)
{
    unsigned int cur = lpTex->Format.dword[0];
    lpTex->Format.dword[0] = enable ? (cur | 0xFC) : (cur & 0xFFFFFF03);
}
