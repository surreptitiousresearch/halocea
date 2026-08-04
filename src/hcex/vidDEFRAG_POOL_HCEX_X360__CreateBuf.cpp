#include "../headers/ws/vid/vidDEFRAG_POOL_HCEX_X360.h"
#include "../headers/ws/vid/vidDEFRAG_BUF_HCEX_X360.h"
#include "../headers/d3d_render_boundary.h" // D3DFMT_INDEX16

void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug-tagged new (Saber)
extern "C" void *dlCalloc(unsigned int num, unsigned int size, const char *file, unsigned int line); // boundary — dlmalloc calloc

// Xbox 360 XG hardware-buffer-header helpers (boundary — XDK, not reversed).
extern "C" void XGSetIndexBufferHeader(unsigned int length, unsigned int offset, _D3DFORMAT format,
                                        unsigned int endian, unsigned int pool, D3DIndexBuffer *outHeader);
extern "C" void XGSetVertexBufferHeader(unsigned int length, unsigned int offset, unsigned int endian,
                                         unsigned int pool, D3DVertexBuffer *outHeader);
/* XGOffsetResourceAddress declared in d3d_render_boundary.h (plain linkage) — do not redeclare */

// vidDEFRAG_POOL_HCEX_X360::CreateBuf @ 0x823F1830
// Heap-allocate a vidDEFRAG_BUF_HCEX_X360 (debug-tagged new, halo_render.cpp:115), then dlCalloc a
// pair of 0x20-byte D3D hardware buffer headers and alias both onto `ptr`: an index-buffer header
// (16-bit indices) and a vertex-buffer header, both offset to the same physical memory range so
// the one buffer can be bound either way depending on what content it holds.
apDEFRAG_BUF *vidDEFRAG_POOL_HCEX_X360::CreateBuf(char *ptr, int size)
{
    vidDEFRAG_BUF_HCEX_X360 *buf = new ("D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 0x73)
        vidDEFRAG_BUF_HCEX_X360();
    if (!buf)
        return nullptr;

    buf->pIBuf = (D3DIndexBuffer *)dlCalloc(1, 0x20,
        "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 0x75);
    buf->pVBuf = (D3DVertexBuffer *)dlCalloc(1, 0x20,
        "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp", 0x76);

    XGSetIndexBufferHeader(size, 0xC, D3DFMT_INDEX16, 0, 0, buf->pIBuf);
    XGOffsetResourceAddress((D3DResource *)buf->pIBuf, ptr);

    XGSetVertexBufferHeader(size, 0xC, 0, 0, buf->pVBuf);
    XGOffsetResourceAddress((D3DResource *)buf->pVBuf, ptr);

    return buf;
}
