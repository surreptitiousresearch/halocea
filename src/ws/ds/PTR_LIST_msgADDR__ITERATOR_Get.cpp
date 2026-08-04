#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::ITERATOR::Get @ 0x825575A0
template<>
msgADDR *ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR::Get() const
{
    return this->pNode;
}
