#include "dsVECTOR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

class strmVID_MEM_BLOCK; // boundary — strm subsystem video-memory block (element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK*,8>::_internalRealloc @ 0x826981B0
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<strmVID_MEM_BLOCK *, 8>::_internalRealloc(int size)
{
    this->pData = (strmVID_MEM_BLOCK **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
