#include "dsVECTOR.h"
#include "../msg/saLIST.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<saLIST,8>::_internalRealloc @ 0x82AC5C00
// Resize the backing store to `size` elements via the debug reallocator (bare relocation; the
// container treats the element as trivially relocatable here, matching Realloc). Element size 24.
template<>
void dsVECTOR<saLIST, 8>::_internalRealloc(int size)
{
    this->pData = (saLIST *)dlRealloc(this->pData, 24 * size, this->__cl.file, this->__cl.line);
}
