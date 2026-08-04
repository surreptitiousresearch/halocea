#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct sslOBJECT; // boundary — ssl scripting object; element is a pointer.

// dsVECTOR<sslOBJECT*,8>::_internalFree @ 0x82AD7B30
// Free a raw storage block previously obtained from the debug allocator.
template<>
void dsVECTOR<sslOBJECT *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
