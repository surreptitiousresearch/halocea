#include "dsVECTOR.h"

#include "dsMsgAddrMapDUMMY.h"

// dsVECTOR<...msgADDR-map DUMMY*,8>::Reserve @ 0x82559EA8
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<dsMsgAddrMapDUMMY *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
