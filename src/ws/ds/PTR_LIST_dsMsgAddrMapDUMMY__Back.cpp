#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::Back @ 0x825585C8
// See the ABI-role CAVEAT in src/ws/ds/PTR_LIST_msgADDR__Front.cpp.
template<>
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::ITERATOR
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::Back()
{
    ITERATOR result;
    result.pNode = this->pTail;
    return result;
}
