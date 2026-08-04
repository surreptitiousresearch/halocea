#include "dsNODE_CACHE.h"

// ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY — a hash-map
// node. DB-verified 16-byte layout: while free it is threaded through the intrusive list by
// {pNext@0, pPrev@4}; those bytes alias the ITEM's {next,key} and the rest is the value payload.
struct dsCineActorMapDUMMY {
    dsCineActorMapDUMMY *pNext;  // 0x00 free-list forward link
    dsCineActorMapDUMMY *pPrev;  // 0x04 free-list back link
    char                 payload[8]; // 0x08 ITEM value bytes (unused by the allocator)
};

// ds debug array allocator (operator new[] with a source-location cookie). Boundary.
void *operator new[](size_t size, const char *file, int line);

// dsNODE_CACHE<...cine-actor-map DUMMY>::AddNodes @ 0x823B8280
// Allocate a fresh block of `nNode` nodes (16 on first use), thread each onto the free list, and
// record the block in `nodeList`. Returns 1 on success, 0 if the array allocation failed.
template<>
int dsNODE_CACHE<dsCineActorMapDUMMY>::AddNodes()
{
    if (!this->nNode)
        this->nNode = 16;

    unsigned int allocSize = 16 * this->nNode;
    if ((unsigned int)this->nNode > 0xFFFFFFFu)
        allocSize = (unsigned int)-1;

    dsCineActorMapDUMMY *nodes =
        (dsCineActorMapDUMMY *)::operator new[](allocSize, this->__cl.file, this->__cl.line);
    if (!nodes)
        return 0;

    dsCineActorMapDUMMY *node = nodes;
    for (int count = 0; count < this->nNode; ++count) {
        if (this->freeNodeList.pHead) {
            dsCineActorMapDUMMY *tail = this->freeNodeList.pTail;
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
