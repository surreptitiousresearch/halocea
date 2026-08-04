#include "dsVECTOR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct apCOUNTER; // boundary — ap performance counter; element is a pointer.

// dsVECTOR<apCOUNTER*,8>::_internalRealloc @ 0x827142B0
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<apCOUNTER *, 8>::_internalRealloc(int size)
{
    this->pData = (apCOUNTER **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
