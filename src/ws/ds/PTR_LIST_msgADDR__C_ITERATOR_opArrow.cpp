#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::C_ITERATOR::operator-> @ 0x82555FE8
template<>
const msgADDR *ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::C_ITERATOR::operator->() const
{
    return this->pNode;
}
