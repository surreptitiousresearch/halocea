#include "dsVECTOR.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

// dsVECTOR<dscBRAND*,8>::Reserve @ 0x826648B8
// Grow the backing store to at least `size` elements when it isn't already that large; a no-op
// when capacity is already sufficient.
template<>
void dsVECTOR<dscBRAND *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
