#include "dsVECTOR.h"

struct propBASE; // boundary — ws-engine prop: root property/component object (element is a pointer)

// dsVECTOR<propBASE*,8>::Reserve @ 0x826C7A10
// Grow the backing store to at least `size` elements when it isn't already that large; a no-op
// when capacity is already sufficient.
template<>
void dsVECTOR<propBASE *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
