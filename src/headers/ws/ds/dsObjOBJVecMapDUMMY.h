#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */
// ws-engine ds: node record for ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,
// dsNODE_CACHE_ALLOCATOR>::DUMMY — the node-pool allocator's free-list bookkeeping type. Same
// 28-byte footprint as the map's ITEM record (next@0/pPrev@4 alias ITEM::next/key, and the
// payload aliases ITEM::value): while a node is FREE it is threaded through the intrusive
// doubly-linked free list via {pNext@0, pPrev@4}; the remaining bytes are unused by the
// allocator. Given its own tag (distinct from ITEM) to avoid ODR collision with other MAP<...>
// DUMMY records of the same byte size — layout/ABI-identical to, but not literally the same C++
// type as, the perpetually-incomplete nested `ds::MAP<...>::DUMMY` (see MAP.h's comment on this).

typedef struct dsObjOBJVecMapDUMMY {
    dsObjOBJVecMapDUMMY *pNext;      // 0x00 free-list forward link
    dsObjOBJVecMapDUMMY *pPrev;      // 0x04 free-list back link
    char                  payload[20]; // 0x08 ITEM value bytes (unused by the allocator)
} dsObjOBJVecMapDUMMY;
