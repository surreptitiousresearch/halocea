#include "dsVECTOR.h"

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::Length @ 0x823ED540
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<animINST *, 8>::Length() const
{
    return this->nElem;
}
