#include "dsVECTOR.h"

struct inpDEVICE; // boundary — inp-subsystem input device, used only as the element pointer type

// dsVECTOR<inpDEVICE*,8>::Length @ 0x823C18F8
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<inpDEVICE *, 8>::Length() const
{
    return this->nElem;
}
