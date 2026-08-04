#include "dsNODE_CACHE.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsNODE_CACHE<dsObjOBJVecMapDUMMY>::Release(NODE*) @ 0x826850C0
// Return `pNode` to the free list directly (intrusive doubly-linked "append after tail" idiom,
// same as AddNodes' per-node linking and Delete/Clear's node-retiring splice).
template<>
void dsNODE_CACHE<dsObjOBJVecMapDUMMY>::Release(dsObjOBJVecMapDUMMY *pNode)
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
