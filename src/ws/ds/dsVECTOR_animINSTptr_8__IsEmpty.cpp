#include "dsVECTOR.h"

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::IsEmpty @ 0x8321B1D0
// True when there are no live elements.
template<>
bool dsVECTOR<animINST *, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
