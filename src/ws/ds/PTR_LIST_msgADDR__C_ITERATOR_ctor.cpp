#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::C_ITERATOR::C_ITERATOR @ 0x82556068
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::C_ITERATOR::C_ITERATOR(msgADDR *node)
{
    this->pNode = node;
}
