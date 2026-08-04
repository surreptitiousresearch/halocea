#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::Back @ 0x82557628
// See the ABI-role CAVEAT in PTR_LIST_msgADDR__Front.cpp (this file's sret/this roles are the
// same corrected-from-decompiler mapping).
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::Back()
{
    ITERATOR result;
    result.pNode = this->pTail;
    return result;
}
