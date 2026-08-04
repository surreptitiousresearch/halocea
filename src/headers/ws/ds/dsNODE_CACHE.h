#pragma once
#include "dsVECTOR.h"
#include "PTR_LIST.h"
#include "../../apCL.h"
// ws-engine ds: free-list allocator/cache of fixed NODE records (used to back ds::MAP hash
// tables). DB-verified layout (types_members dsNODE_CACHE<...DUMMY>):
//   nNode@0, nodeList@4, freeNodeList@24, __cl@36 — size 44.

template<class NODE>
struct dsNODE_CACHE {
    int                  nNode;        // 0x00 total node count
    dsVECTOR<NODE *, 8>  nodeList;     // 0x04 owned node blocks
    ds::PTR_LIST<NODE>   freeNodeList; // 0x18 available (unused) nodes
    apCL                 __cl;         // 0x24 allocation call-site cookie

    // 0x82557BF8 (msgADDR-map DUMMY) — zero-init nNode/nodeList/freeNodeList and copy the
    // caller's call-site cookie into __cl. Deviation: the raw decompile ("local variable
    // allocation has failed") also shows __cl.file/.line being overwritten with `this`/&cl
    // (nonsensical for an apCL) — reconstructed as the obviously-intended `__cl = *cl` copy,
    // matching the sibling dsNODE_CACHE_ALLOCATOR/MAP ctors' apCL-by-pointer idiom. CAVEAT.
    dsNODE_CACHE(apCL cl);
    // 0x8255B6A0 — release all node blocks (Term) then destroy nodeList.
    ~dsNODE_CACHE();

    // 0x8255BC10 (msgADDR-map DUMMY) — take one free node (growing the pool if empty).
    NODE *GetEmpty();
    // 0x82559B70 (msgADDR-map DUMMY) — return `pNode` to the free list (tail-insert).
    void Release(NODE *pNode);
    // 0x8255A8F0 (msgADDR-map DUMMY) — clear the free list, `operator delete[]` every owned node
    // block, drop nodeList's live count to 0 and shrink its backing storage to match, reset nNode.
    void Term();

protected:
    // 0x823B8280 / 0x82689518 — allocate a fresh block of `nNode` free nodes (16 on first use),
    // thread each onto the intrusive free list, and record the block in `nodeList`. Returns 1 on
    // success, 0 if the array allocation failed. Reversed per-instantiation.
    int AddNodes();
};
