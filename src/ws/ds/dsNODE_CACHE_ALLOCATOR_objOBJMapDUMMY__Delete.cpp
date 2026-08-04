#include "dsNODE_CACHE_ALLOCATOR.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsNODE_CACHE_ALLOCATOR<dsObjOBJVecMapDUMMY>::Delete(NODE*) @ 0x82686450
// Return `pNode` to the free list directly. Identical logic to the base dsNODE_CACHE::Release —
// the compiler emitted a separate copy for the allocator's own Delete entry point rather than
// forwarding, reproduced faithfully rather than delegating (matches the disassembly's duplicate
// code body).
template<>
void dsNODE_CACHE_ALLOCATOR<dsObjOBJVecMapDUMMY>::Delete(dsObjOBJVecMapDUMMY *pNode)
{
    if (!this->freeNodeList.pHead) {
        pNode->pPrev = nullptr;
        pNode->pNext = nullptr;
        this->freeNodeList.pHead = pNode;
        this->freeNodeList.pTail = pNode;
        ++this->freeNodeList.length;
        return;
    }

    dsObjOBJVecMapDUMMY *pTail = this->freeNodeList.pTail;
    dsObjOBJVecMapDUMMY *pTailNext = pTail->pNext;
    pNode->pPrev = pTail;
    pNode->pNext = pTailNext;
    pTail->pNext = pNode;
    if (!pNode->pNext) {
        this->freeNodeList.pTail = pNode;
        ++this->freeNodeList.length;
        return;
    }
    pNode->pNext->pPrev = pNode;
    ++this->freeNodeList.length;
}
