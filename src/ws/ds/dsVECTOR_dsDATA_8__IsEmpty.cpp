#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::IsEmpty @ 0x8325F938
// True when the vector currently holds zero live elements.
template<>
bool dsVECTOR<dsDATA, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
