#include "dsVECTOR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

struct sslOBJECT; // boundary — ssl scripting object; element is a pointer.

// dsVECTOR<sslOBJECT*,8>::_internalRealloc @ 0x82AD7B58
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<sslOBJECT *, 8>::_internalRealloc(int size)
{
    this->pData = (sslOBJECT **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
