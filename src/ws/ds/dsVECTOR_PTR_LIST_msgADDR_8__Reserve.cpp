#include "dsVECTOR.h"
#include "PTR_LIST.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::Reserve @ 0x8255A650
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
