#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::Reserve @ 0x825A94F8
// Grow the backing storage to at least `size` elements (no-op if already large enough).
template<>
void dsVECTOR<dsDATA, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
