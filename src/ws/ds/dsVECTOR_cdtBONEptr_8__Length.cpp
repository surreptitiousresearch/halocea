#include "dsVECTOR.h"

struct cdtBONE; // boundary — ws-engine cdt: collision-detection skeleton bone (element is a pointer)

// dsVECTOR<cdtBONE*,8>::Length @ 0x828B88E8
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<cdtBONE *, 8>::Length() const
{
    return this->nElem;
}
