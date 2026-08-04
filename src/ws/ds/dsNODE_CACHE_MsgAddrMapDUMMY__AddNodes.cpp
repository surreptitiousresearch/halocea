#include "dsNODE_CACHE.h"
#include "dsMsgAddrMapDUMMY.h"

// ds debug array allocator (operator new[] with a source-location cookie). Boundary.
void *operator new[](size_t size, const char *file, int line);

// dsNODE_CACHE<...msgADDR-map DUMMY>::AddNodes @ 0x8255B2F8
// Allocate a fresh block of `nNode` 12-byte nodes (16 on first use), append each to the free list,
// and record the block in `nodeList`. Returns 1 on success, 0 if the array allocation failed.
// (The decompiler renders the per-node link as a ds::PTR_LIST::Insert at the tail iterator; that is
// the inlined tail-append reproduced directly below, matching the sibling AddNodes instantiations.)
template<>
int dsNODE_CACHE<dsMsgAddrMapDUMMY>::AddNodes()
{
    if (!this->nNode)
        this->nNode = 16;

    unsigned int allocSize = 12 * this->nNode;
    if ((unsigned int)this->nNode > 0x15555555u)
        allocSize = (unsigned int)-1;

    dsMsgAddrMapDUMMY *nodes =
        (dsMsgAddrMapDUMMY *)::operator new[](allocSize, this->__cl.file, this->__cl.line);
    if (!nodes)
        return 0;

    dsMsgAddrMapDUMMY *node = nodes;
    for (int count = 0; count < this->nNode; ++count) {
        if (this->freeNodeList.pHead) {
            dsMsgAddrMapDUMMY *tail = this->freeNodeList.pTail;
            node->pPrev = tail;
            node->pNext = tail->pNext;
            tail->pNext = node;
            if (node->pNext)
                node->pNext->pPrev = node;
            else
                this->freeNodeList.pTail = node;
        } else {
            node->pPrev = 0;
            node->pNext = 0;
            this->freeNodeList.pHead = node;
            this->freeNodeList.pTail = node;
        }
        ++node;
        ++this->freeNodeList.length;
    }

    this->nodeList.PushBack(nodes);
    return 1;
}
