#include "dsNODE_CACHE.h"
#include "dsMsgAddrMapDUMMY.h"

// dsNODE_CACHE<dsMsgAddrMapDUMMY>::Release @ 0x82559B70
// Return `pNode` to the free list (tail-insert), matching the intrusive PTR_LIST<T> API.
template<>
void dsNODE_CACHE<dsMsgAddrMapDUMMY>::Release(dsMsgAddrMapDUMMY *pNode)
{
    ds::PTR_LIST<dsMsgAddrMapDUMMY>::ITERATOR tailIter;
    tailIter.pNode = freeNodeList.pTail;
    freeNodeList.Insert(pNode, tailIter);
}
