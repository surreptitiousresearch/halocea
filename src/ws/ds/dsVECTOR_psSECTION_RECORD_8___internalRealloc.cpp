#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<psSECTION_RECORD,8>::_internalRealloc @ 0x82516728
// Resize the backing store to hold exactly `size` elements via the debug reallocator, tagged
// with this vector's allocation call-site cookie. psSECTION_RECORD is trivially relocatable, so
// this is a bare dlRealloc with no per-element copy/move. Element size 20 bytes.
template<>
void dsVECTOR<psSECTION_RECORD, 8>::_internalRealloc(int size)
{
    this->pData = (psSECTION_RECORD *)dlRealloc(this->pData, 20 * size, this->__cl.file, this->__cl.line);
}
