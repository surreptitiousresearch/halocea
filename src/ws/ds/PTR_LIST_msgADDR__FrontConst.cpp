#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::Front (const, C_ITERATOR overload)
// @ 0x825580F8 -- header names this "FrontConst" (msgADDR instantiation only). See the ABI-role
// CAVEAT in PTR_LIST_msgADDR__Front.cpp.
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::C_ITERATOR
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::FrontConst()
{
    C_ITERATOR result;
    result.pNode = this->pHead;
    return result;
}
