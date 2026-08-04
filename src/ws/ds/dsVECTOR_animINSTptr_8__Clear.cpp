#include "dsVECTOR.h"

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::Clear @ 0x823ED598
// Pointer elements are trivially destructible, so the generic per-element destructor loop is
// empty and this collapses to resetting the live count.
template<>
void dsVECTOR<animINST *, 8>::Clear()
{
    this->nElem = 0;
}
