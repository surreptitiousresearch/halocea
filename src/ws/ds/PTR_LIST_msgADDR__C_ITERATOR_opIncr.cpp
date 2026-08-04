#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::C_ITERATOR::operator++ @ 0x82557650
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::C_ITERATOR &
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::C_ITERATOR::operator++()
{
    this->pNode = ds::LIST_NODE_ACCESS_POLICY<msgADDR>::Next(this->pNode);
    return *this;
}
