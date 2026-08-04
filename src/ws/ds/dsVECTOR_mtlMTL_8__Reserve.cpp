#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"

// dsVECTOR<mtlMTL,8>::Reserve @ 0x82688470
// Grow the backing store to at least `size` elements; a no-op if already big enough.
template<>
void dsVECTOR<mtlMTL, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
