/* hcex_dyn_ibuf_get @0x823EFD98 — fetch the D3D index buffer backing the ws-engine dynamic hardware
 * buffer for index content. vidVBUF_MNG is the ws-engine vertex/index buffer manager (boundary);
 * vidVBMng is its global instance. The index buffer handle sits at byte 0x60 within the returned
 * dynamic-hardware-buffer block. */

#include "../headers/d3d_boundary.h"

typedef struct vidVBUF_MNG vidVBUF_MNG;

extern vidVBUF_MNG *vidVBMng;
extern void *vidVBUF_MNG__GetDynHwB(vidVBUF_MNG *self, int content_kind);

extern "C" D3DIndexBuffer *hcex_dyn_ibuf_get(void)
{
    return *(D3DIndexBuffer **)((char *)vidVBUF_MNG__GetDynHwB(vidVBMng, 2) + 0x60);
}
