/* ?EnableGamma@txmD3D_TEX@@UAAXH@Z @0x827BC5B0 */
#include "../../headers/ws/txm/txmD3D_TEX.h"
#include "../../headers/ws/txm/txm_vid_boundary.h" /* vidDriver / vidDRIVER */
#include "../../headers/d3dDRIVER_INTERFACE.h"
#include "../../headers/d3d_xenon_format_boundary.h"

// 0x827BC5B0 -- ?EnableGamma@txmD3D_TEX@@UAAXH@Z
// Toggle gamma-correct hardware sampling on this render target. Records the flag in the base
// txmTEXTURE, then -- only when the texture's state bit 0x400000 is set -- patches the low byte of
// the GPU fetch constant's first dword (the sign/gamma control field: 0xFC bits set to enable,
// cleared to disable). When the filter-state bit 0x10000000 is also set, it additionally remaps
// the format's low 6-bit DataFormat field to its filtered 16bpp (enable) or 8bpp (disable)
// sibling. Finally re-pushes the sRGB sampler state through the D3D driver interface.
void txmD3D_TEX::EnableGamma(int enable)
{
    this->txmTEXTURE::EnableGamma(enable);

    if (((this->state.state >> 22) & 1) != 0)
    {
        D3DBaseTexture *tex = this->GetD3DTex();
        unsigned int ctrl = tex->Format.dword[0];
        tex->Format.dword[0] = enable ? (ctrl | 0xFC) : (ctrl & 0xFFFFFF03);

        if (((this->stateFltr.state >> 28) & 1) != 0)
        {
            D3DBaseTexture *tex2 = this->GetD3DTex();
            GPUTEXTUREFORMAT curFmt = (GPUTEXTUREFORMAT)(tex2->Format.dword[1] & 0x3F);
            unsigned char filtered = enable ? (unsigned char)_d3dXenonMake16bppFilteredFormat(curFmt)
                                            : (unsigned char)_d3dXenonMake8bppFilteredFormat(curFmt);
            tex2->Format.dword[1] = (tex2->Format.dword[1] & 0xFFFFFFC0) | (filtered & 0x3F);
        }
    }

    ((d3dDRIVER_INTERFACE *)vidDriver->drvInterface)->UpdateTextureStateSRGB(this);
}
