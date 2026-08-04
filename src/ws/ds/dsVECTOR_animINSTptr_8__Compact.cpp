#include "dsVECTOR.h"

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::Compact @ 0x8287F148
// Shrink the backing storage exactly to the live element count.
template<>
void dsVECTOR<animINST *, 8>::Compact()
{
    this->Realloc(this->nElem);
}
