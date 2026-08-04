#include "dsVECTOR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::_internalRealloc @ 0x82681640
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<dsObjOBJVecMapITEM *, 8>::_internalRealloc(int size)
{
    this->pData = (dsObjOBJVecMapITEM **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
