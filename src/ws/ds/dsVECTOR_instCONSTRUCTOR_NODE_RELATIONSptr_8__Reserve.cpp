#include "dsVECTOR.h"

struct instCONSTRUCTOR_NODE_RELATIONS; // boundary — ws-engine prop: instance-constructor node relation record (element is a pointer)

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS*,8>::Reserve @ 0x826E1A18
// Grow the backing store to at least `size` elements when it isn't already that large; a no-op
// when capacity is already sufficient.
template<>
void dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
