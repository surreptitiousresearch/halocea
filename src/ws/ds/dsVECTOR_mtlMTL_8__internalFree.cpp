#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"

extern "C" void dlFree(void *ptr); // boundary

// dsVECTOR<mtlMTL,8>::_internalFree @ 0x826810D0
// Free a backing-storage block. mtlMTL elements have already been destroyed by the caller
// (Clear/destructor), so this is a bare heap free.
template<>
void dsVECTOR<mtlMTL, 8>::_internalFree(void *p)
{
    dlFree(p);
}
