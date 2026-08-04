#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::Clear @ 0x82557CA8
// Clear returns void (no sret), so this=r3, no ABI-role swap. Walks the list unlinking each node
// (using dsMsgAddrMapDUMMY's direct pNext/pPrev fields -- this instantiation's policy has no -4
// embedded-node adjustment, unlike msgADDR's) and resets the list to empty.
template<>
void ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::Clear()
{
    dsMsgAddrMapDUMMY *node;
    while (this->pHead)
    {
        node = this->pHead;
        this->pHead = this->pHead->pNext;
        node->pPrev = nullptr;
        node->pNext = nullptr;
    }
    this->pTail = nullptr;
    this->length = 0;
}
