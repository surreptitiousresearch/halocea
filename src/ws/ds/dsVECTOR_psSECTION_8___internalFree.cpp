#include "dsVECTOR.h"

class psSECTION; // boundary — ps subsystem section handle (element by value)

extern "C" void dlFree(void *ptr); // boundary — debug heap free

// dsVECTOR<psSECTION,8>::_internalFree @ 0x82516690
// Release a raw backing-store allocation previously obtained through the debug heap.
template<>
void dsVECTOR<psSECTION, 8>::_internalFree(void *pStorage)
{
    dlFree(pStorage);
}
