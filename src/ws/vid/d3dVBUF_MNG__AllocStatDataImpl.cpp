/* ?AllocStatDataImpl@d3dVBUF_MNG@@UAAPAUvidDEFRAG_CHUNK@@W4vidBUF_CONTENT@@V?$apSTATE_T@E@@HPAX@Z @0x827CFF28 */
#include "../../headers/ws/vid/d3dVBUF_MNG.h"
#include "../../headers/ws/vid/vidDEFRAG_CHUNK_TEX.h"
#include "../../headers/ws/vid/vidDEFRAG_CHUNK_OBJ_X360.h"
#include "../../headers/ws/ap/apDEFRAG_MNG.h"
#include "../../headers/ws/ap/apCOUNTER.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// 0x827CFF28 -- ?AllocStatDataImpl@d3dVBUF_MNG@@UAAPAUvidDEFRAG_CHUNK@@W4vidBUF_CONTENT@@
//               V?$apSTATE_T@E@@HPAX@Z
// Concrete backing allocator behind vidVBUF_MNG::AllocStatData (reversed in
// vidVBUF_MNG__AllocStatData.cpp). Allocates the defrag chunk whose concrete type matches the
// requested buffer-content kind: a texture chunk for VID_BC_TEX, an object-backed vertex/index
// chunk for VID_BC_VERT / VID_BC_INDEX. Records the owning object in the chunk's +0x20 slot,
// stamps the content kind, then adds the chunk's size (in megabytes) to the per-content
// "cnt_da" defrag-arena memory counter.
//
// ABI note: the mangled signature passes `flags` (apSTATE_T<unsigned char>) by value, but the
// parameter is entirely unused by this body; declared as a pointer to match the boundary
// vidVBUF_MNG::AllocStatDataImpl virtual it overrides.

extern apDEFRAG_MNG apDefragMng; // ?apDefragMng@@3PAVapDEFRAG_MNG@@A
extern "C" apCOUNTER    cnt_da;      // defrag-arena per-content memory counter

// 1 / (1024*1024): bytes -> megabytes (matches literal 0.00000095367432 in the disasm).
static const float BYTES_TO_MB = 0.00000095367432f;

vidDEFRAG_CHUNK *d3dVBUF_MNG::AllocStatDataImpl(vidBUF_CONTENT cont, apSTATE_T<unsigned char> *flags,
                                                int size, void *auxData)
{
    vidDEFRAG_CHUNK *chunk = nullptr;

    if (cont == VID_BC_TEX)
    {
        chunk = apDefragMng.Alloc<vidDEFRAG_CHUNK_TEX>(size);
        // Shipped code stores auxData into the chunk's +0x20 slot unconditionally (aliases pTex
        // for the texture chunk); modeled through the object view since auxData is an objOBJ*.
        ((vidDEFRAG_CHUNK_OBJ *)chunk)->pObj = (objOBJ *)auxData;
    }
    if (cont == VID_BC_VERT || cont == VID_BC_INDEX)
    {
        chunk = apDefragMng.Alloc<vidDEFRAG_CHUNK_OBJ_X360>(size);
        ((vidDEFRAG_CHUNK_OBJ *)chunk)->pObj = (objOBJ *)auxData;
    }

    if (!IGNORE_STRONG_ASSERT && !chunk)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("pChunk",
            "D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\d3d_rnd_vbuf_8_xenon.cpp",
            842, empty_string);

    chunk->content = cont;

    float prev = cnt_da.GetValue(cont);
    cnt_da.SetValue(cont, (float)chunk->size * BYTES_TO_MB + prev);
    return chunk;
}
