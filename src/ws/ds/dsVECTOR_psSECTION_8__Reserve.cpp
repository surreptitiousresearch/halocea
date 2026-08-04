#include "dsVECTOR.h"

class psSECTION; // boundary — ps subsystem section handle (element by value)

// dsVECTOR<psSECTION,8>::Reserve @ 0x8251BC20
// Grow the backing store to hold at least `size` elements (no-op if already large enough).
template<>
void dsVECTOR<psSECTION, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
