#include "dsVECTOR.h"

struct apCOUNTER; // boundary — ap performance counter; element is a pointer.

// dsVECTOR<apCOUNTER*,8>::Reserve @ 0x827156A8
// Grow the backing store to at least `size` elements; a no-op if already large enough.
template<>
void dsVECTOR<apCOUNTER *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
