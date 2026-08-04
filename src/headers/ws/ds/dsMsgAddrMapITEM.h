#pragma once
#include "MAP.h"
#include "../msg/msgADDR.h"
// ws-engine ds: alias for ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::ITEM (the
// map's chained hash-bucket node — see MAP.h for the DB-verified next@0/key@4/value@8 layout).
// `dsMsgAddrMapITEM` is a plain typedef (NOT a distinct struct) so every
// `dsVECTOR<dsMsgAddrMapITEM*,8>` specialization written against this tag is, to the compiler and
// linker, the SAME type as `ds::MAP<int,msgADDR*,...>::ITEM` — required for ds::MAP's own `data`
// bucket vector (typed `dsVECTOR<ITEM*,8>` via the nested spelling) to resolve to these bodies.

typedef ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM dsMsgAddrMapITEM;
