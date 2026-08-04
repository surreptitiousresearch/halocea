#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::_internalFree @ 0x823ED610
// Free a raw storage block previously obtained from the debug allocator.
template<>
void dsVECTOR<animINST *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
