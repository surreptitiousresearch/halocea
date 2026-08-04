#include "dsVECTOR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::_internalRealloc @ 0x82698170
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<strmVID_BUFFER *, 8>::_internalRealloc(int size)
{
    this->pData = (strmVID_BUFFER **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
