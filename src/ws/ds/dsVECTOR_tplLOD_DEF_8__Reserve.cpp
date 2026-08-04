#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"

// dsVECTOR<tplLOD_DEF,8>::Reserve @ 0x82B0DAE0
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<tplLOD_DEF, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
