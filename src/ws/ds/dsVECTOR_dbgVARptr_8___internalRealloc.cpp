#include "dsVECTOR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

class dbgVAR; // boundary — dbg subsystem debug variable record (element is a pointer)

// dsVECTOR<dbgVAR*,8>::_internalRealloc @ 0x8255DD68
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<dbgVAR *, 8>::_internalRealloc(int size)
{
    this->pData = (dbgVAR **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
