#include "dsVECTOR.h"

struct sslFOREACH_DATA; // boundary — ssl foreach-loop cursor record; element is a value.

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<sslFOREACH_DATA,8>::_internalFree @ 0x825C9E00
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<sslFOREACH_DATA, 8>::_internalFree(void *p)
{
    dlFree(p);
}
