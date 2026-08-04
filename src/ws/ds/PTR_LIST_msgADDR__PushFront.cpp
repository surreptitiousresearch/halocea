#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::PushFront @ 0x82557808
// See the ABI-role CAVEAT in PTR_LIST_msgADDR__Front.cpp. Links `val` in as the new head via the
// policy's SetNext/SetPrev (the -4 embedded-node adjustment), matching the raw trace exactly.
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::PushFront(msgADDR *val)
{
    if (this->pHead)
    {
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(val, nullptr);
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext(val, this->pHead);
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(this->pHead, val);
        this->pHead = val;
    }
    else
    {
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(val, nullptr);
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext(val, nullptr);
        this->pHead = val;
        this->pTail = val;
    }
    ++this->length;

    ITERATOR result;
    result.pNode = val;
    return result;
}
