#include "dsVECTOR.h"
#include "dsSTRID.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsSTRID,8>::_internalRealloc @ 0x823CFD08
// Resize the backing store to `size` elements via the debug reallocator. dsSTRID is trivially
// relocatable (a single interned const char*), so this is a bare dlRealloc. Element size 4 bytes.
template<>
void dsVECTOR<dsSTRID, 8>::_internalRealloc(int size)
{
    this->pData = (dsSTRID *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
