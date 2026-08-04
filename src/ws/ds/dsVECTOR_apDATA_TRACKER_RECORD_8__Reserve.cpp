#include "dsVECTOR.h"
#include "../ap/apDATA_TRACKER.h"

// dsVECTOR<apDATA_TRACKER::RECORD,8>::Reserve @ 0x82A24F08
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<apDATA_TRACKER::RECORD, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
