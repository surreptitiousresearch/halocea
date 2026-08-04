#include "dsVECTOR.h"
#include "../ssl/sslCLASS_ELEMENT.h"

// dsVECTOR<sslCLASS_ELEMENT,8>::Reserve @ 0x82A3B728
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<sslCLASS_ELEMENT, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
