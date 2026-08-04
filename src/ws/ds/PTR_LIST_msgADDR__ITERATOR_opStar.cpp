#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::ITERATOR::operator* @ 0x82509E18
template<>
msgADDR &ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR::operator*() const
{
    return *this->pNode;
}
