#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::IsEmpty @ 0x82555FC8
template<>
bool ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::IsEmpty() const
{
    return this->pHead == nullptr;
}
