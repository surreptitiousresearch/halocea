#pragma once
#include "../rend/rendORD.h"
#include "vidOBJ_SPLIT.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsARRAY.h"
#include "../ap/apSTATE_T.h"
// ws-engine vid: render-order object used for split (multi-material) geometry — the concrete
// type objGEOM_UNSHARED::pRendData actually points to when a model has material splits (rendORD
// is its abstract/partial base). DB-verified layout (types_members vidORD): rendORD@0 (base, 52B
// padded from the base's own 50B), idxBuf@52 (vidOBJ_IB, 8B), vBuf@60 (ds::ARRAY<vidOBJ_VB,5>,
// 20B), tcTile@80 (ds::ARRAY<unsigned short,5>, 10B), vBufStride@90 (ds::ARRAY<char,5>, 5B),
// objOwnedD3dDataInBuf@95 (apSTATE_T<unsigned char>, 1B), splitList@96
// (dsVECTOR<vidOBJ_SPLIT,8>, 20B), isSplitsShared@116 (bool, 1B).
//
// CAVEAT: only splitList is in the closure of the MTL_DESC_DB batch that motivated this header
// (MTL_DESC_DB::_SelectMaterialsByInstAffixes reaches it via `(vidORD*)pGeom->pRendData ->
// splitList[i]`, confirmed against disasm: `addi r26, r11(pRendData), 0x60` where 0x60 == 96 ==
// offsetof(vidORD, splitList)). idxBuf/vBuf/tcTile/vBufStride/objOwnedD3dDataInBuf are kept as an
// opaque byte blob per the established vid* boundary convention (large self-contained
// video-driver classes reached through a handful of methods) rather than resolving
// vidOBJ_IB/vidOBJ_VB/vidOBJ_B recursively.

struct vidDEFRAG_CHUNK; // ../vid/vidDEFRAG_CHUNK.h -- pointer only

// DB-verified layout (types_members vidOBJ_B, size 4) -- defrag-managed device buffer handle.
typedef struct vidOBJ_B {
    vidDEFRAG_CHUNK *pChunk; // 0x00
} vidOBJ_B;

// DB-verified layout (types_members vidOBJ_VB, size 4) -- vertex buffer (vidOBJ_B base only).
struct vidOBJ_VB : vidOBJ_B {
};

// DB-verified layout (types_members vidOBJ_IB, size 8) -- index buffer.
struct vidOBJ_IB : vidOBJ_B {
    int offsetIB; // 0x04
};

struct vidORD : rendORD {
    vidOBJ_IB                   idxBuf;               // 0x34 index buffer handle
    ds::ARRAY<vidOBJ_VB, 5>     vBuf;                 // 0x3C per-stream vertex buffer handles
    ds::ARRAY<unsigned short, 5> tcTile;              // 0x50 per-stream texcoord tiling
    ds::ARRAY<char, 5>          vBufStride;           // 0x5A per-stream vertex strides
    apSTATE_T<unsigned char>    objOwnedD3dDataInBuf; // 0x5F
    dsVECTOR<vidOBJ_SPLIT, 8>   splitList;            // 0x60 per-split material-slot lists (parallel to objSPLIT::entryList)
    bool                        isSplitsShared;       // 0x74
};
