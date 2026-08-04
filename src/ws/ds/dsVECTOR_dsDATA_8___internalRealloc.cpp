#include "dsVECTOR.h"
#include "dsDATA.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsDATA,8>::_internalRealloc @ 0x8256A988
// Resize the backing store to `size` elements via the debug reallocator. Live elements above the
// new size are assumed to already have been torn down by the caller (Realloc calls ShrinkResize
// first); this is a bare dlRealloc, matching every other instantiation.
template<>
void dsVECTOR<dsDATA, 8>::_internalRealloc(int size)
{
    this->pData = (dsDATA *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
