#include "dsVECTOR.h"

struct inpDEVICE; // boundary — ws-engine inp: physical input device (element is a pointer)

// dsVECTOR<inpDEVICE*,8>::Reserve @ 0x82641650
// Grow the backing store to at least `size` elements when it isn't already that large; a no-op
// when capacity is already sufficient.
template<>
void dsVECTOR<inpDEVICE *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
