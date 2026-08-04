#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct apCOUNTER; // boundary — ap performance counter; element is a pointer.

// dsVECTOR<apCOUNTER*,8>::_internalFree @ 0x827141B8
// Free a raw storage block previously obtained from the debug allocator.
template<>
void dsVECTOR<apCOUNTER *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
