#pragma once
#include "MAP.h"
#include "../msg/msgADDR.h"
// ws-engine ds: ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY — the map's
// 12-byte hash-bucket node record. DB-verified size (types_members ds::MAP<...>::DUMMY) matches
// ds::MAP<...>::ITEM exactly (next@0/key@4/value@8): while an ITEM is LIVE it is read/written
// through the ITEM view (next/key/value); while FREE (on dsNODE_CACHE::freeNodeList) it is
// threaded through the ds::PTR_LIST<DUMMY> intrusive link view (pNext@0/pPrev@4), which is why
// the DB models it as a separate DUMMY record instead of reusing ITEM directly — the two views
// alias the same bytes.
//
// CORRECTED: MAP.h leaves ds::MAP<K,V,H,C,A>::DUMMY an INCOMPLETE nested type in the generic
// template (each instantiation's concrete layout is unrelated to the others' — dsSTRID's map uses
// a different DUMMY, objOBJ's map uses a different one again). This is the msgADDR instantiation's
// EXPLICIT SPECIALIZATION of that nested member, giving it a real body. `dsMsgAddrMapDUMMY` below
// is a plain ALIAS (typedef, not a distinct struct — an earlier draft of this header wrongly
// defined a brand-new top-level struct here, which is a DIFFERENT C++ type from
// `ds::MAP<...>::DUMMY` and would not link against ds::MAP's own method bodies, which reach this
// type only via the nested spelling `allocator.GetEmpty()` etc.). Every `dsVECTOR<dsMsgAddrMapDUMMY*,8>`
// / `dsNODE_CACHE<dsMsgAddrMapDUMMY>` specialization written against this tag is, to the compiler
// and linker, the SAME type as `ds::MAP<int,msgADDR*,...>::DUMMY`.

namespace ds {

template<>
struct MAP<int, msgADDR *, HASH, CMP, dsNODE_CACHE_ALLOCATOR>::DUMMY {
    DUMMY *pNext;      // 0x00 free-list forward link (aliases ITEM::next)
    DUMMY *pPrev;      // 0x04 free-list back link (aliases ITEM::key, an int, while free)
    char   payload[4]; // 0x08 aliases ITEM::value (msgADDR*) while free; unused by the allocator
};

} // namespace ds

typedef ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::DUMMY dsMsgAddrMapDUMMY;
