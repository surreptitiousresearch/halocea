#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::C_ITERATOR::IsDone @ 0x82507F78
// A real compiled body exists for this instantiation (not inlined, unlike the generic template's
// inline default), but its semantics match the generic default exactly.
template<>
bool ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::C_ITERATOR::IsDone() const
{
    return this->pNode == nullptr;
}
