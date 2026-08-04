#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<sslCLASS_MAP_ELEM,8>::_internalRealloc @ 0x825C9FC8
// Resize the backing store to `size` elements via the debug reallocator (bare relocation; the
// container treats the element as trivially relocatable here, matching Realloc). Element size 8.
template<>
void dsVECTOR<sslCLASS_MAP_ELEM, 8>::_internalRealloc(int size)
{
    this->pData = (sslCLASS_MAP_ELEM *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
