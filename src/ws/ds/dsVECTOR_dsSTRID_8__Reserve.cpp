#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::Reserve @ 0x823DFB78
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<dsSTRID, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
