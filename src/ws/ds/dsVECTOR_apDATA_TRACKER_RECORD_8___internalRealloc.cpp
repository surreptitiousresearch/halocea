#include "dsVECTOR.h"
#include "../ap/apDATA_TRACKER.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<apDATA_TRACKER::RECORD,8>::_internalRealloc @ 0x82A242E0
// Resize the backing store to `size` elements via the debug reallocator (bare relocation; the
// container treats the 124-byte element as trivially relocatable here, matching Realloc).
template<>
void dsVECTOR<apDATA_TRACKER::RECORD, 8>::_internalRealloc(int size)
{
    this->pData = (apDATA_TRACKER::RECORD *)dlRealloc(this->pData, 124 * size, this->__cl.file, this->__cl.line);
}
