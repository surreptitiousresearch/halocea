#include "dsVECTOR.h"
#include "../msg/saLIST.h"

// dsVECTOR<saLIST,8>::Reserve @ 0x82AC6B38
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<saLIST, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
