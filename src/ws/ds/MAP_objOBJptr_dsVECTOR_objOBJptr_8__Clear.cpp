#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only
#include "dsObjOBJVecMapDUMMY.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Clear() @ 0x82688D38
// Walk every bucket; for each live chain, free every node's value-vector storage and splice the
// whole chain onto the allocator's free list one node at a time (same append-after-tail idiom as
// Delete/AddNodes), then reset bucket count to 0. Bucket *capacity* (`data.nElem`, the vector's
// live-slot count — bucket heads, not entries) is left unchanged; only `count` (entry count) and
// the bucket head pointers are cleared.
//
// DEVIATION: as in Delete, the raw decompile types the walked nodes as `objOBJ*`/generic DUMMY
// pointers via coincidental struct-size aliasing; re-typed to `dsObjOBJVecMapDUMMY*` throughout.
template<>
void ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Clear()
{
    int bucketCount = this->data.nElem;
    for (int bucket = 0; bucket < bucketCount; ++bucket) {
        ITEM *node = this->data[bucket];
        if (!node)
            continue;

        dsObjOBJVecMapDUMMY *dummyNode = reinterpret_cast<dsObjOBJVecMapDUMMY *>(node);
        do {
            dsObjOBJVecMapDUMMY *next = dummyNode->pNext; // node->next, aliased
            dlFree(*reinterpret_cast<void **>(dummyNode->payload)); // node->value.pData

            if (this->allocator.freeNodeList.pHead) {
                dsObjOBJVecMapDUMMY *pTail = reinterpret_cast<dsObjOBJVecMapDUMMY *>(this->allocator.freeNodeList.pTail);
                dsObjOBJVecMapDUMMY *tailNext = pTail->pNext;
                dummyNode->pPrev = pTail;
                dummyNode->pNext = tailNext;
                pTail->pNext = dummyNode;
                if (dummyNode->pNext)
                    dummyNode->pNext->pPrev = dummyNode;
                else
                    this->allocator.freeNodeList.pTail = reinterpret_cast<DUMMY *>(dummyNode);
            } else {
                dummyNode->pPrev = nullptr;
                dummyNode->pNext = nullptr;
                this->allocator.freeNodeList.pHead = reinterpret_cast<DUMMY *>(dummyNode);
                this->allocator.freeNodeList.pTail = reinterpret_cast<DUMMY *>(dummyNode);
            }
            ++this->allocator.freeNodeList.length;

            dummyNode = next;
        } while (dummyNode);
    }

    // DEVIATION: not just `count = 0` — the decompile also collapses the bucket vector's live
    // length to 0 (`data.nElem = 0`, the inlined tail of dsVECTOR<ITEM*,8>::Clear() on `data`
    // itself). The physical backing array/capacity are untouched (freed lazily by ~MAP or reused
    // by a later Insert/HashResize), but logically there are zero buckets until something resizes
    // `data` again.
    this->data.nElem = 0;
    this->count = 0;
}
