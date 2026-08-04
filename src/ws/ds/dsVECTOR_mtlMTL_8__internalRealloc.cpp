#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<mtlMTL,8>::_internalRealloc @ 0x82681440
// Resize the backing store to `size` elements via the debug reallocator (element size 60 bytes).
template<>
void dsVECTOR<mtlMTL, 8>::_internalRealloc(int size)
{
    this->pData = (mtlMTL *)dlRealloc(this->pData, 60 * size, this->__cl.file, this->__cl.line);
}
