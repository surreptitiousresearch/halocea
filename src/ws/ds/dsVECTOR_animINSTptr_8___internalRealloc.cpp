#include "dsVECTOR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::_internalRealloc @ 0x823ED680
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<animINST *, 8>::_internalRealloc(int size)
{
    this->pData = (animINST **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
