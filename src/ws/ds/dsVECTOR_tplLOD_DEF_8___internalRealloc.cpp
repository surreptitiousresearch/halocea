#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<tplLOD_DEF,8>::_internalRealloc @ 0x82B0CDE0
// Resize the backing store to `size` elements via the debug reallocator. tplLOD_DEF is trivially
// relocatable, so this is a bare dlRealloc. Element size 4 bytes.
template<>
void dsVECTOR<tplLOD_DEF, 8>::_internalRealloc(int size)
{
    this->pData = (tplLOD_DEF *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
