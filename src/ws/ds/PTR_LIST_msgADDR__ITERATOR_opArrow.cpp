#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::ITERATOR::operator-> @ 0x8250B098
// Not declared in the generic PTR_LIST.h template (only Get()/operator*/operator++ were listed for
// ITERATOR there) but the DB has a real compiled instance for this instantiation, so it is added
// here as a genuine per-instantiation member.
template<>
msgADDR *ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR::operator->() const
{
    return this->pNode;
}
