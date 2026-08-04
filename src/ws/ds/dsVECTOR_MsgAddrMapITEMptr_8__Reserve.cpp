#include "dsVECTOR.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::Reserve @ 0x8255A608
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<dsMsgAddrMapITEM *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
