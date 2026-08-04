#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::ITERATOR::operator++ @ 0x82558128
// Advances via the policy's Next() (the -4 embedded-node adjustment), not a raw field read.
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR &
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR::operator++()
{
    this->pNode = ds::LIST_NODE_ACCESS_POLICY<msgADDR>::Next(this->pNode);
    return *this;
}
