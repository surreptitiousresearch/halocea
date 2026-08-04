#include "dsVECTOR.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)

// dsVECTOR<camCAMERA*,8>::Reserve @ 0x82704918
// Grow the backing store to at least `size` elements when it isn't already that large; a no-op
// when capacity is already sufficient.
template<>
void dsVECTOR<camCAMERA *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
