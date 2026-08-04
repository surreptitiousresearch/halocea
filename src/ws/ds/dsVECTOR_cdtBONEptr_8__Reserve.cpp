#include "dsVECTOR.h"

struct cdtBONE; // boundary — ws-engine cdt: collision-detection skeleton bone (element is a pointer)

// dsVECTOR<cdtBONE*,8>::Reserve @ 0x828BD1C0
// Grow the backing store to at least `size` elements when it isn't already that large; a no-op
// when capacity is already sufficient.
template<>
void dsVECTOR<cdtBONE *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
