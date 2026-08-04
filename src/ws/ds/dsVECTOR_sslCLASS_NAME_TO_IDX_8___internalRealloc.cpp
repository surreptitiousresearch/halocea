#include "dsVECTOR.h"
#include "../ssl/sslCLASS_NAME_TO_IDX.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<sslCLASS::NAME_TO_IDX,8>::_internalRealloc @ 0x82A37B20
// Resize the backing store to `size` elements via the debug reallocator. NAME_TO_IDX is
// trivially relocatable, so this is a bare dlRealloc. Element size 8 bytes.
template<>
void dsVECTOR<ssl_class::NAME_TO_IDX, 8>::_internalRealloc(int size)
{
    this->pData = (ssl_class::NAME_TO_IDX *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
