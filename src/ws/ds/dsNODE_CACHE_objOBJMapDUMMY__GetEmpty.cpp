#include "dsNODE_CACHE.h"

// boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY,
// a hash-map node record keying object pointers to per-key object vectors. Opaque here; only
// linked and handed out by pointer. Same tag as dsVECTOR_objOBJMapDUMMYptr_8__ShrinkResize.
struct dsObjOBJVecMapDUMMY;

// dsNODE_CACHE<...objOBJ-vec map DUMMY>::GetEmpty @ 0x8268A2F8
// Return a free node, growing the pool first if the free list is empty; the node is unlinked
// from the free list (the inlined Erase relinks pPrev/pNext and updates pHead/pTail/length)
// before being handed out.
// CORRECTED (msgADDR-map batch, DB-verified via ds::PTR_LIST<DUMMY,...>::Erase's real mangle):
// Erase takes its ITERATOR BY VALUE and RETURNS the unlinked T* — not `void Erase(const ITERATOR*)`
// as an earlier pass on this generic PTR_LIST template had it; call site fixed to match.
template<>
dsObjOBJVecMapDUMMY *dsNODE_CACHE<dsObjOBJVecMapDUMMY>::GetEmpty()
{
    if (!this->freeNodeList.pHead)
        this->AddNodes();
    dsObjOBJVecMapDUMMY *emptyNode = this->freeNodeList.pHead;
    this->freeNodeList.Erase(ds::PTR_LIST<dsObjOBJVecMapDUMMY>::ITERATOR(emptyNode));
    return emptyNode;
}
