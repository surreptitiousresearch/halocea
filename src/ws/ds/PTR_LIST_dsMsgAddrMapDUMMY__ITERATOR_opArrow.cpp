#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::ITERATOR::operator-> @ 0x825585F0
// Real compiled instance for this instantiation, added to the generic ITERATOR template per the
// same rationale as PTR_LIST_msgADDR__ITERATOR_opArrow.cpp (see PTR_LIST.h note added there).
template<>
dsMsgAddrMapDUMMY *ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::ITERATOR::operator->() const
{
    return this->pNode;
}
