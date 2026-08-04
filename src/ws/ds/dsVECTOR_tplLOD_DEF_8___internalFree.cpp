#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<tplLOD_DEF,8>::_internalFree @ 0x827DDCB8
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<tplLOD_DEF, 8>::_internalFree(void *p)
{
    dlFree(p);
}
