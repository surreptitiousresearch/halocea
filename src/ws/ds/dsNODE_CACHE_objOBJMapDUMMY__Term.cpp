#include "dsNODE_CACHE.h"
#include "dsObjOBJVecMapDUMMY.h"

// operator delete[] boundary — releases a block previously obtained from operator new[].
extern void operator delete[](void *ptr);

// dsNODE_CACHE<dsObjOBJVecMapDUMMY>::Term() @ 0x82688C78
// Unlink every free-list node (leaving the list empty), `operator delete[]` every owned node
// block recorded in `nodeList`, drop nodeList's live count to 0 and shrink its backing storage to
// match (Realloc(0)), then reset nNode to 0.
template<>
void dsNODE_CACHE<dsObjOBJVecMapDUMMY>::Term()
{
    dsObjOBJVecMapDUMMY *node = this->freeNodeList.pHead;
    while (node) {
        dsObjOBJVecMapDUMMY *next = node->pNext;
        node->pPrev = nullptr;
        this->freeNodeList.pHead = next;
        node = next;
    }
    this->freeNodeList.pTail = nullptr;
    this->freeNodeList.length = 0;

    int blockCount = this->nodeList.nElem;
    for (int i = 0; i < blockCount; ++i)
        operator delete[](this->nodeList[i]);

    this->nodeList.nElem = 0;
    this->nodeList.Realloc(0);
    this->nNode = 0;
}
