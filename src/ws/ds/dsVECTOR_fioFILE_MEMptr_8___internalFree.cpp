#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct fioFILE_MEM; // boundary — fio in-memory file; element is a pointer.

// dsVECTOR<fioFILE_MEM*,8>::_internalFree @ 0x82AC73E0
// Free a raw storage block previously obtained from the debug allocator.
template<>
void dsVECTOR<fioFILE_MEM *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
