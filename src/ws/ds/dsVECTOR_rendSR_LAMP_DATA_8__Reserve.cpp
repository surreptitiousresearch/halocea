#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"

// dsVECTOR<rendSR_LAMP_DATA,8>::Reserve @ 0x8252DFE0
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
