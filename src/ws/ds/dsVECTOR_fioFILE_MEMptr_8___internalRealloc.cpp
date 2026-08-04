#include "dsVECTOR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct fioFILE_MEM; // boundary — fio in-memory file; element is a pointer.

// dsVECTOR<fioFILE_MEM*,8>::_internalRealloc @ 0x82AC73A0
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<fioFILE_MEM *, 8>::_internalRealloc(int size)
{
    this->pData = (fioFILE_MEM **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
