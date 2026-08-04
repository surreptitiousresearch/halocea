#include "../../headers/ws/txm/txmD3D_TEX.h"

// XDK: rebase a resource's D3D header onto a caller-owned memory block. boundary.
extern void *XGOffsetResourceAddress(void *pResource, void *pBaseAddress);

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
    XGOffsetResourceAddress(this->lpSysSurf, this->pData);
    this->RecreateSplitScreen();
    return 0;
}
