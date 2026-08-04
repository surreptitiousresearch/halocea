#include "../../headers/ws/vid/vidVBUF_MNG.h"
#include "../ds/ds_assert_boundary.h"
#include <string.h>

// ap error log (?_apLog@@YAXPBDZZ). boundary.
extern void _apLog(const char *fmt, ...);

// 0x827B8BC8 -- ?WriteStatData@vidVBUF_MNG@@...
// Write `size` bytes of `data` into `pChunk` at byte `offset`: lock the chunk for writing, memcpy
// into the returned base (or log a lock failure), then unlock.
//
// DEVIATION: the decompiler manufactures ~15 trailing int parameters (a6..a20) that are actually
// the STRONG_ASSERT2_HELPER::asserd argument scratch (the stack slots that stage &size/&offset for
// the debug log). disasm 0x827B8BC8 confirms the real ABI is (this, pChunk, offset, data, size).
// LockWrite is vtbl slot 0x8, Unlock slot 0xC.
void vidVBUF_MNG::WriteStatData(vidDEFRAG_CHUNK *pChunk, int offset, const void *data, int size)
{
    if (!IGNORE_STRONG_ASSERT && (size <= 0 || offset < 0 || offset + size > pChunk->size))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "size > 0 && offset >= 0 && offset + size <= pChunk->size",
            "D:\\Projects\\code\\common\\src.sys\\drv\\video\\vid_vbuf.cpp",
            722,
            "size", size,
            "offset", offset,
            "pChunk->size", pChunk->size);

    char *base = (char *)pChunk->__vftable->LockWrite(pChunk);
    if (base)
        memcpy(&base[offset], data, size);
    else
        _apLog("~Error,D3D~Can't lock buffer!");

    pChunk->__vftable->Unlock(pChunk);
}
