#include "dsVECTOR.h"

class dbgVAR; // boundary — dbg subsystem debug variable record (element is a pointer)

// dsVECTOR<dbgVAR*,8>::Reserve @ 0x8255FD48
// Grow the backing store to at least `size` elements; a no-op if already large enough.
template<>
void dsVECTOR<dbgVAR *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
