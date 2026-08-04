#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<m3dMATR,8>::_internalFree @ 0x823ED640
// m3dMATR is a plain POD (union of floats) — no per-element teardown, just release storage.
template<>
void dsVECTOR<m3dMATR, 8>::_internalFree(void *p)
{
    dlFree(p);
}
