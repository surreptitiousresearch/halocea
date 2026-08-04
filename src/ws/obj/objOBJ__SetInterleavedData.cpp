#include "objOBJ.h"
// 0x82BCB5D0  ?SetInterleavedData@objOBJ@@QAAXPAXH_K@Z
// Store the interleaved vertex buffer, its per-vertex size, and its FVF into the shared geometry.
// NOTE: mangle is (PAX, H, _K) = (void*, int, unsigned __int64); the decompiler mis-split the
// last two args. Verified against the disassembly (stw @0x8C, stb @0xB3, std @0x10).

void objOBJ::SetInterleavedData(void *interleavedData, int size, unsigned __int64 fvf)
{
    _UnshareGeom();
    pGeom->pSharedGeom->pDataInterleaved = interleavedData;
    pGeom->pSharedGeom->interleavedSize = (unsigned __int8)size;
    pGeom->pSharedGeom->fvfInterleaved = fvf;
}
