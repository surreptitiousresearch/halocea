#include "dsVECTOR.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<camCAMERA*,8>::_internalFree @ 0x82703B80
// Free a backing-storage block (protected helper; pointer elements need no per-element teardown).
template<>
void dsVECTOR<camCAMERA *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
