#include "dsVECTOR.h"
#include "../vid/vidOBJ_SPLIT.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<vidOBJ_SPLIT,8>::_internalRealloc @ 0x82C86558
// Resize the backing store to `size` elements via the debug reallocator (bare relocation; the
// container treats the 284-byte element as trivially relocatable here, matching Realloc).
template<>
void dsVECTOR<vidOBJ_SPLIT, 8>::_internalRealloc(int size)
{
    this->pData = (vidOBJ_SPLIT *)dlRealloc(this->pData, 284 * size, this->__cl.file, this->__cl.line);
}
