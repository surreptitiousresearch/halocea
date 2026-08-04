#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::Erase @ 0x82558608
// Erase returns a plain T* -- no hidden sret, no ABI-role swap (this=r3, where=r4 by value), same
// as the msgADDR instantiation's Erase. CORRECTED (matches the PTR_LIST.h fix noted in
// dsNODE_CACHE_MsgAddrMapDUMMY__GetEmpty.cpp): returns the unlinked node pointer, not void.
template<>
dsMsgAddrMapDUMMY *ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::Erase(ITERATOR where)
{
    dsMsgAddrMapDUMMY *node = where.pNode;
    dsMsgAddrMapDUMMY *prevNode = ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::Prev(node);
    dsMsgAddrMapDUMMY *nextNode = ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::Next(node);

    if (prevNode)
        ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetNext(prevNode, nextNode);
    else
    {
        this->pHead = nextNode;
        if (this->pHead)
            ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetPrev(this->pHead, nullptr);
    }

    if (nextNode)
        ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetPrev(nextNode, prevNode);
    else
    {
        this->pTail = prevNode;
        if (this->pTail)
            ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetNext(this->pTail, nullptr);
    }

    ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetPrev(node, nullptr);
    ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetNext(node, nullptr);
    --this->length;
    return node;
}
