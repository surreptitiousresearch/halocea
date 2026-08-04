#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"

// dsVECTOR<sslCLASS_MAP_ELEM,8>::Reserve @ 0x825CF948
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<sslCLASS_MAP_ELEM, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
