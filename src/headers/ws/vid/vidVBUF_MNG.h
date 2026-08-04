#pragma once
// ws-engine vid: vertex/index-buffer manager. DB-verified layout (types_members vidVBUF_MNG,
// size 932): derives vidMEM_MNG@0, then an FVF-descriptor MRU cache + full descriptor table, the
// live object list, and two defrag-chunk vectors (all static allocations + those currently in
// use). Previously modeled as an opaque boundary typedef in vid_boundary.h; this promotes it to a
// full DB-verified layout so the AllocStatData/WriteStatData methods reversed this batch can name
// their fields. Most methods (FitFVF/DynGeomDraw/DiscardDynAll/AllocStatDataImpl/...) remain
// boundary; see vid_boundary.h and the notes below.

#include "vidMEM_MNG.h"
#include "vidDEFRAG_CHUNK.h"
#include "../ap/apSTATE_T.h"
#include "../ap/apDEFRAG_CHUNK.h"
#include "../ds/dsVECTOR.h"

struct vidFVF_DESCR; // boundary -- cached flexible-vertex-format descriptor
struct objOBJ;       // boundary -- render object

typedef struct vidVBUF_MNG : vidMEM_MNG
{
    vidFVF_DESCR                     *fvfCacheLastUsed[5]; /* 0x034 (52)  MRU cache */
    vidFVF_DESCR                     *fvfList[200];        /* 0x048 (72)  descriptor table */
    dsVECTOR<objOBJ *, 8>             objList;             /* 0x368 (872) */
    dsVECTOR<apDEFRAG_CHUNK *, 8>     chunks;              /* 0x37C (892) all static chunks (sorted) */
    dsVECTOR<apDEFRAG_CHUNK *, 8>     chunkUsed;           /* 0x390 (912) chunks currently in use */

    // 0x827B9CE8 -- reversed in vidVBUF_MNG__AllocStatData.cpp. Allocates a static buffer chunk via
    // AllocStatDataImpl and, on success, records it in the sorted `chunks` vector.
    vidDEFRAG_CHUNK *AllocStatData(vidBUF_CONTENT cont, apSTATE_T<unsigned char> *flags,
                                   int size, void *auxData);

    // Backing allocator for AllocStatData (creates the vidDEFRAG_CHUNK). boundary.
    vidDEFRAG_CHUNK *AllocStatDataImpl(vidBUF_CONTENT cont, apSTATE_T<unsigned char> *flags,
                                       int size, void *auxData);

    // 0x827B8BC8 -- reversed in vidVBUF_MNG__WriteStatData.cpp. Locks the chunk, memcpy's `size`
    // bytes of `data` at `offset`, then unlocks.
    void WriteStatData(vidDEFRAG_CHUNK *pChunk, int offset, const void *data, int size);

    // 0x827B7890 -- reversed in vidVBUF_MNG__FitFVF.cpp. Find-or-create the cached FVF descriptor
    // for a plain interleaved fvf mask (full=interleaved=fvf, multistream=instanced=0). Scans the
    // MRU cache then the descriptor table, promoting hits; on miss builds one into the first free
    // fvfList slot.
    vidFVF_DESCR *FitFVF(unsigned long long fvf);

    // 0x827B7A88 -- reversed in vidVBUF_MNG__FitFVF4.cpp. Overload distinguishing the four fvf
    // variants explicitly. NOTE: the decompiler renders this as a 5-argument function; the disasm
    // shows only four 64-bit register args (r4..r7) forwarded to vidFVF_DESCR::Init -- the fifth
    // "argument" is a register-pair aliasing artifact.
    vidFVF_DESCR *FitFVF(unsigned long long fvf_full, unsigned long long fvf_multistream,
                         unsigned long long fvf_interleaved, unsigned long long fvf_instanced);
} vidVBUF_MNG; /* 932 bytes */
