#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::_internalFree @ 0x82698098
// Free a raw storage block previously obtained from the debug allocator.
template<>
void dsVECTOR<strmVID_BUFFER *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
