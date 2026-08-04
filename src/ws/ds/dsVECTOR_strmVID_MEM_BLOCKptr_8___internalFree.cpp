#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

class strmVID_MEM_BLOCK; // boundary — strm subsystem video-memory block (element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK*,8>::_internalFree @ 0x826980C0
// Free a raw storage block previously obtained from the debug allocator.
template<>
void dsVECTOR<strmVID_MEM_BLOCK *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
