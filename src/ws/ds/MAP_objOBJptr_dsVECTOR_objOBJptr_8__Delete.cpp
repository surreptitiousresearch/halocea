#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only
#include "dsObjOBJVecMapDUMMY.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Delete(DUMMY*)
// @ 0x82688200
// Free `ptr`'s value-vector storage (the retiring ITEM's payload), then return it to the
// allocator's free list directly (bypasses bucket unlinking; for callers that have already
// unlinked the node themselves) — same intrusive doubly-linked "append after tail" idiom as
// dsNODE_CACHE<DUMMY>::AddNodes, but for a single externally-supplied node.
//
// DEVIATION: the raw decompile types the parameter as `objOBJ*` and reads/writes objOBJ fields
// (`ptr->pGeom`, `ptr->id`, `ptr->name`) that happen to alias, byte-for-byte, DUMMY's real layout
// (dsObjOBJVecMapDUMMY: pNext@0/pPrev@4, ITEM::value.pData@8) purely from the decompiler's type
// propagation picking a same-size unrelated struct — re-typed to `dsObjOBJVecMapDUMMY*` and its
// real fields throughout (see MAP.h's note: the per-instantiation opaque tag stands in for the
// perpetually-incomplete nested `ds::MAP<...>::DUMMY`).
template<>
void ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Delete(DUMMY *ptrArg)
{
    dsObjOBJVecMapDUMMY *ptr = reinterpret_cast<dsObjOBJVecMapDUMMY *>(ptrArg);

    // ptr is really the retiring ITEM reinterpreted as DUMMY; ITEM::value.pData lands at the same
    // offset (0x08, `payload`'s start) as DUMMY's opaque payload.
    dlFree(*reinterpret_cast<void **>(ptr->payload));

    if (this->allocator.freeNodeList.pHead) {
        dsObjOBJVecMapDUMMY *pTail = reinterpret_cast<dsObjOBJVecMapDUMMY *>(this->allocator.freeNodeList.pTail);
        dsObjOBJVecMapDUMMY *tailNext = pTail->pNext;
        ptr->pPrev = pTail;
        ptr->pNext = tailNext;
        pTail->pNext = ptr;
        if (ptr->pNext)
            ptr->pNext->pPrev = ptr;
        else
            this->allocator.freeNodeList.pTail = reinterpret_cast<DUMMY *>(ptr);
    } else {
        ptr->pPrev = nullptr;
        ptr->pNext = nullptr;
        this->allocator.freeNodeList.pHead = reinterpret_cast<DUMMY *>(ptr);
        this->allocator.freeNodeList.pTail = reinterpret_cast<DUMMY *>(ptr);
    }
    ++this->allocator.freeNodeList.length;
}
