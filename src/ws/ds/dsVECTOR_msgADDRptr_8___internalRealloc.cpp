#include "dsVECTOR.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem); only a pointer is handled

// Debug global reallocator the ws build links in: dlRealloc(ptr, size, file, line).
extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<msgADDR*,8>::_internalRealloc @ 0x82556C40
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<msgADDR *, 8>::_internalRealloc(int size)
{
    this->pData = (msgADDR **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
