#pragma once
// d3dVBUF_MNG — the concrete D3D (Xbox 360) implementation of the vertex/index-buffer manager;
// the leaf class whose vtable slots back the boundary vidVBUF_MNG virtuals. DB-verified layout
// (types_members d3dVBUF_MNG, size 952): base vidVBUF_MNG@0 (size 932), then IBWritesLog@932
// (a deferred index-buffer-write log). Only AllocStatDataImpl is reversed here; the rest of the
// D3D-specific overrides remain boundary.
#include "vidVBUF_MNG.h"
#include "../ds/dsVECTOR.h"
#include "../ap/apSTATE_T.h"

// d3dVBUF_MNG::IB_WRITE_OP — DB-verified (types_members d3dVBUF_MNG::IB_WRITE_OP, size 12): an
// anonymous target-descriptor union @0 (boundary $HASH), size @4, srcData (const void *) @8. The
// union body is not needed by any reversed method; kept opaque as a raw 4-byte cell.
typedef struct d3dVBUF_MNG_IB_WRITE_OP {
    unsigned int _target; // 0x00 anonymous union (boundary)
    int          size;    // 0x04
    const void  *srcData; // 0x08
} d3dVBUF_MNG_IB_WRITE_OP;

struct d3dVBUF_MNG : vidVBUF_MNG {
    dsVECTOR<d3dVBUF_MNG_IB_WRITE_OP, 8> IBWritesLog; // 0x3A4 (932) deferred IB-write log

    // 0x827CFF28 (?AllocStatDataImpl@d3dVBUF_MNG@@UAAPAUvidDEFRAG_CHUNK@@W4vidBUF_CONTENT@@
    // V?$apSTATE_T@E@@HPAX@Z) — backing allocator for vidVBUF_MNG::AllocStatData. Allocates the
    // concrete defrag chunk for the requested content kind, records the owning object, and bumps
    // the per-content "cnt_da" memory counter. Reversed in d3dVBUF_MNG__AllocStatDataImpl.cpp.
    vidDEFRAG_CHUNK *AllocStatDataImpl(vidBUF_CONTENT cont, apSTATE_T<unsigned char> *flags,
                                       int size, void *auxData) /* override */;

    // 0x827CEC10 -- reversed in d3dVBUF_MNG__GetDynHwB.cpp. Virtual. Return the platform hardware
    // buffer header that precedes the current dynamic vidBUFFER for `cont` (0x68 bytes before it),
    // or nullptr when no buffer is bound.
    void *GetDynHwB(vidBUF_CONTENT cont) const;

    // 0x827CD738 -- reversed in d3dVBUF_MNG__DiscardDynAll.cpp. Virtual. Reset the write cursor of
    // both dynamic (vertex + index) buffers to zero, discarding the frame's dynamic allocations.
    void DiscardDynAll();
};
