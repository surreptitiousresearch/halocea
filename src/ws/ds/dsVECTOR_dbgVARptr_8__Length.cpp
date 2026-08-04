#include "dsVECTOR.h"

class dbgVAR; // boundary — dbg subsystem debug variable record (element is a pointer)

// dsVECTOR<dbgVAR*,8>::Length @ 0x8255DA60
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<dbgVAR *, 8>::Length() const
{
    return this->nElem;
}
