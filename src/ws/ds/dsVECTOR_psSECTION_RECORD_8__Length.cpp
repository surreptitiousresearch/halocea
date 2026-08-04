#include "dsVECTOR.h"

struct psSECTION_RECORD; // boundary — ps (particle system) section record (element by value)

// dsVECTOR<psSECTION_RECORD,8>::Length @ 0x82516038
// Number of live elements.
template<>
int dsVECTOR<psSECTION_RECORD, 8>::Length() const
{
    return this->nElem;
}
