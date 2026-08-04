#include "dsVECTOR.h"
#include "../ssl/sslCLASS_NAME_TO_IDX.h"

// dsVECTOR<sslCLASS::NAME_TO_IDX,8>::Reserve @ 0x82A3A8C0
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<ssl_class::NAME_TO_IDX, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
