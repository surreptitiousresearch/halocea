#include "dsVECTOR.h"

class psSECTION; // boundary — ps subsystem section handle (element by value)

// dsVECTOR<psSECTION,8>::Compact @ 0x8251BC38
// Shrink the backing store to exactly the live element count.
template<>
void dsVECTOR<psSECTION, 8>::Compact()
{
    this->Realloc(this->nElem);
}
