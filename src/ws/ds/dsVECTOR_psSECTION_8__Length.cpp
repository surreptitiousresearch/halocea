#include "dsVECTOR.h"

class psSECTION; // boundary — ps subsystem section handle (element by value)

// dsVECTOR<psSECTION,8>::Length @ 0x82516030
// Number of live elements.
template<>
int dsVECTOR<psSECTION, 8>::Length() const
{
    return this->nElem;
}
