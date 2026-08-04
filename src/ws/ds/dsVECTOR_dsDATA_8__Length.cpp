#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::Length @ 0x825233A8
// Number of live elements.
template<>
int dsVECTOR<dsDATA, 8>::Length() const
{
    return this->nElem;
}
