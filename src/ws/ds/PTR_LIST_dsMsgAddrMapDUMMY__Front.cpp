#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::Front @ 0x82557D90
// See the ABI-role CAVEAT in src/ws/ds/PTR_LIST_msgADDR__Front.cpp -- the same hidden-sret/this
// register swap applies to this instantiation's ITERATOR-by-value-returning methods too
// (DB-verified via decompile: same `this->pHead = result->pNode; return this;` raw-trace shape).
template<>
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::ITERATOR
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::Front()
{
    ITERATOR result;
    result.pNode = this->pHead;
    return result;
}
