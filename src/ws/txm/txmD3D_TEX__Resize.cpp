/* ?Resize@txmD3D_TEX@@UAAIII@Z @0x827BC270 */
#include "../../headers/ws/txm/txmD3D_TEX.h"
#include "../../headers/d3d_render_boundary.h"

// DEVIATION: this TU redeclared XGOffsetResourceAddress locally as
// `void *XGOffsetResourceAddress(void *, void *)` — wrong in the return type and in the first
// parameter. Under C++ that was a legal OVERLOAD of the shared d3d_render_boundary.h declaration,
// so it compiled while calling a mangled symbol nothing defines; the collision only became visible
// once the declaration was given C linkage. DB applied_types @XGOffsetResourceAddress:
// `void XGOffsetResourceAddress(D3DResource *pResource, void *pBaseAddress);`

// 0x827BC270 -- ?Resize@txmD3D_TEX@@UAAIII@Z
// Resize this hardware render-target texture to w x h. The dimensions are stored into the base
// txmTEXTURE's short sizeX/sizeY (hence truncated to 16 bits); the format packing is recomputed,
// the XG texture header rebuilt (yielding the new packed byte size), the system surface re-based
// onto the texture's own pData block, and the split-screen siblings recreated. Always returns 0.
unsigned int txmD3D_TEX::Resize(unsigned int w, unsigned int h)
{
    this->sizeX = (short)w;
    this->sizeY = (short)h;
    this->PrepareSizeFormat();
    this->texSize = this->SetXGHeader(0, this->format, this->lpSysSurf, nullptr, nullptr);
    XGOffsetResourceAddress((D3DResource *)this->lpSysSurf, this->pData);
    this->RecreateSplitScreen();
    return 0;
}
