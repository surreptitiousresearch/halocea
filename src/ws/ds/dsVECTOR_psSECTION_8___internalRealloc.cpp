#include "dsVECTOR.h"

class psSECTION; // boundary — ps subsystem section handle (element by value)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<psSECTION,8>::_internalRealloc @ 0x825166E8
// Resize the raw backing-store allocation to `size` elements via the debug reallocator.
// psSECTION is a trivially relocatable 4-byte handle, so a bare dlRealloc suffices (no
// per-element move). Element size 4 bytes.
template<>
void dsVECTOR<psSECTION, 8>::_internalRealloc(int size)
{
    this->pData = (psSECTION *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
