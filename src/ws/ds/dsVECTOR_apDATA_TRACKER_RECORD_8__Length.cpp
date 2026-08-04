#include "dsVECTOR.h"
#include "../ap/apDATA_TRACKER.h"

// dsVECTOR<apDATA_TRACKER::RECORD,8>::Length @ 0x82A242B8
// Number of live elements.
template<>
int dsVECTOR<apDATA_TRACKER::RECORD, 8>::Length() const
{
    return this->nElem;
}
