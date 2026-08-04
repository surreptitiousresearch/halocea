/* hcex_dyn_vbuf_get @0x823EFE98 — fetch the D3D vertex buffer backing the ws-engine dynamic hardware
 * buffer for vertex content. vidVBUF_MNG is the ws-engine vertex/index buffer manager (boundary);
 * vidVBMng is its global instance. The vertex buffer handle sits at byte 0x64 within the returned
 * dynamic-hardware-buffer block. */

#include "../headers/d3d_boundary.h"

typedef struct vidVBUF_MNG vidVBUF_MNG;

extern vidVBUF_MNG *vidVBMng;
extern void *vidVBUF_MNG__GetDynHwB(vidVBUF_MNG *self, int content_kind);

extern "C" D3DVertexBuffer *hcex_dyn_vbuf_get(void)
{
    return *(D3DVertexBuffer **)((char *)vidVBUF_MNG__GetDynHwB(vidVBMng, 1) + 0x64);
}
