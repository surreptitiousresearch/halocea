#include "../../headers/ws/vid/vidDEFRAG_POOL_HCEX_X360.h"
#include "../../headers/ws/vid/vidDEFRAG_BUF_HCEX_X360.h"
#include "../../headers/d3d_render_boundary.h"

// 0x823F1830 -- ?CreateBuf@vidDEFRAG_POOL_HCEX_X360@@UAAPAUapDEFRAG_BUF@@PADH@Z
// Build the single defrag buffer that backs this pool's 16 MB physical arena. Allocates a
// vidDEFRAG_BUF_HCEX_X360 (via its own ctor, reversed separately), then lays a pair of 0x20-byte
// Xbox 360 hardware buffer headers over the same physical range `ptr`: one index-buffer header and
// one vertex-buffer header. The same memory is thereby aliased as both an IB and a VB, so a chunk
// can be drawn as either without a second allocation.
//
// Deviation from the decompiler: the raw output shows the vidDEFRAG_BUF_HCEX_X360 ctor fully
// inlined (operator new + apDEFRAG_BUF::apDEFRAG_BUF + manual vtable/pIBuf/pVBuf patch, with the
// two 0x20-byte headers dlCalloc'd directly into the +0x60/+0x64 slots). Reconstructed here as the
// equivalent `new vidDEFRAG_BUF_HCEX_X360()` plus the dlCalloc/XG setup, matching the header note.
// (source: D:\Projects\code\HCEX\sources\halo_render.cpp:115)

extern "C" void *dlCalloc(unsigned int num, unsigned int size, const char *file, unsigned int line);

apDEFRAG_BUF *vidDEFRAG_POOL_HCEX_X360::CreateBuf(char *ptr, int size)
{
    vidDEFRAG_BUF_HCEX_X360 *buf = new vidDEFRAG_BUF_HCEX_X360();
    if (!buf)
        return nullptr;

    // 0x20-byte hardware headers for the two aliased views of the physical range.
    buf->pIBuf = (D3DIndexBuffer *)dlCalloc(1u, 0x20u,
        "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 0x75u);
    buf->pVBuf = (D3DVertexBuffer *)dlCalloc(1u, 0x20u,
        "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 0x76u);

    XGSetIndexBufferHeader((unsigned int)size, 0xC, D3DFMT_INDEX16, 0, 0, buf->pIBuf);
    XGOffsetResourceAddress((D3DResource *)buf->pIBuf, ptr);

    XGSetVertexBufferHeader((unsigned int)size, 0xC, 0, 0, buf->pVBuf);
    XGOffsetResourceAddress((D3DResource *)buf->pVBuf, ptr);

    return buf;
}
