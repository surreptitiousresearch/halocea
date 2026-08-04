#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::Length @ 0x8269F370
// Number of live elements.
template<>
int dsVECTOR<dsSTRID, 8>::Length() const
{
    return this->nElem;
}
