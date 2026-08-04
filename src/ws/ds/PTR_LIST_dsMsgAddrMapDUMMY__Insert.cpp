#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::Insert @ 0x82558AB0
// See the ABI-role CAVEAT in src/ws/ds/PTR_LIST_msgADDR__Front.cpp (r3=out iterator, r4=this,
// r5=val, r6=where -- val/where unaffected). Unlike the msgADDR instantiation, this policy's
// pNext/pPrev are plain direct fields (no -4 embedded-node adjustment), so the raw trace reads as
// straightforward pointer patching: DB-verified via disasm (0x82558AB0-BA0), e.g.
// `val->pNext = where->pNext; val->pPrev = where; where->pNext = val;` etc.
template<>
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::ITERATOR
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::Insert(dsMsgAddrMapDUMMY *val, ITERATOR where)
{
    ITERATOR result;

    if (this->pHead)
    {
        dsMsgAddrMapDUMMY *afterWhere = ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::Next(where.pNode);
        ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetNext(val, afterWhere);
        ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetPrev(val, where.pNode);
        ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetNext(where.pNode, val);
        if (afterWhere)
            ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetPrev(afterWhere, val);
        else
            this->pTail = val;
    }
    else
    {
        ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetPrev(val, nullptr);
        ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetNext(val, nullptr);
        this->pHead = val;
        this->pTail = val;
    }

    ++this->length;
    result.pNode = val;
    return result;
}
