#include "dsVECTOR.h"

struct psSECTION_RECORD; // boundary — ps (particle system) section record (element by value)

// dsVECTOR<psSECTION_RECORD,8>::Reserve @ 0x825192E0
// Grow the backing store to at least `size` elements if it isn't already that large.
template<>
void dsVECTOR<psSECTION_RECORD, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
