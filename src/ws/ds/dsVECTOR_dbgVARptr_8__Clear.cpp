#include "dsVECTOR.h"

class dbgVAR; // boundary — dbg subsystem debug variable record (element is a pointer)

// dsVECTOR<dbgVAR*,8>::Clear @ 0x8255DA90
// Pointer elements are trivially destructible, so the generic per-element destructor loop is
// empty and this collapses to resetting the live count.
template<>
void dsVECTOR<dbgVAR *, 8>::Clear()
{
    this->nElem = 0;
}
