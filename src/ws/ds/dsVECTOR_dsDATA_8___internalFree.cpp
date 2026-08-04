#include "dsVECTOR.h"
#include "dsDATA.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

// dsVECTOR<dsDATA,8>::_internalFree @ 0x823C1D40
// Free a backing-storage block. dsDATA elements have already been destroyed by the caller
// (ShrinkResize/Clear/dtor) before this runs, so this is a bare dlFree.
template<>
void dsVECTOR<dsDATA, 8>::_internalFree(void *p)
{
    dlFree(p);
}
