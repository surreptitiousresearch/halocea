#include "dsNODE_CACHE.h"

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY — a hash-map
// node. DB-verified 28-byte layout: while free it is threaded through the intrusive list by
// {pNext@0, pPrev@4}; those bytes alias the ITEM's {next,key} and the rest is the value payload
// (a dsVECTOR<objOBJ*,8>). Distinct tag avoids ODR collision with other MAP DUMMY records.
struct dsObjOBJVecMapDUMMY {
    dsObjOBJVecMapDUMMY *pNext;  // 0x00 free-list forward link
    dsObjOBJVecMapDUMMY *pPrev;  // 0x04 free-list back link
    char                 payload[20]; // 0x08 ITEM value bytes (unused by the allocator)
};

// ds debug array allocator (operator new[] with a source-location cookie). Boundary.
void *operator new[](size_t size, const char *file, int line);

// dsNODE_CACHE<...objOBJ-map DUMMY>::AddNodes @ 0x82689518
// Allocate a fresh block of `nNode` nodes (16 on first use), thread each onto the free list, and
// record the block in `nodeList`. Returns 1 on success, 0 if the array allocation failed.
template<>
int dsNODE_CACHE<dsObjOBJVecMapDUMMY>::AddNodes()
{
    if (!this->nNode)
        this->nNode = 16;

    unsigned int allocSize = 28 * this->nNode;
    if ((unsigned int)this->nNode > 0x9249249u)
        allocSize = (unsigned int)-1;

    dsObjOBJVecMapDUMMY *nodes =
        (dsObjOBJVecMapDUMMY *)::operator new[](allocSize, this->__cl.file, this->__cl.line);
    if (!nodes)
        return 0;

    dsObjOBJVecMapDUMMY *node = nodes;
    for (int count = 0; count < this->nNode; ++count) {
        if (this->freeNodeList.pHead) {
            dsObjOBJVecMapDUMMY *tail = this->freeNodeList.pTail;
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
