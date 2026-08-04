#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::IsEmpty @ 0x8269F358
// True when there are no live elements.
template<>
bool dsVECTOR<dsSTRID, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
