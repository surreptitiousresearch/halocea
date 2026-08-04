#include "dsVECTOR.h"

struct sslFOREACH_DATA; // boundary — ssl foreach-loop cursor record; element is a value.

// dsVECTOR<sslFOREACH_DATA,8>::Reserve @ 0x82AF27B0
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<sslFOREACH_DATA, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
