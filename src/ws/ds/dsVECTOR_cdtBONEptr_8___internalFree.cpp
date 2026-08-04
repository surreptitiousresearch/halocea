#include "dsVECTOR.h"

struct cdtBONE; // boundary — ws-engine cdt: collision-detection skeleton bone (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<cdtBONE*,8>::_internalFree @ 0x828B8AC0
// Free a backing-storage block (protected helper; pointer elements need no per-element teardown).
template<>
void dsVECTOR<cdtBONE *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
