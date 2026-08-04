#include "dsVECTOR.h"
#include "../ssl/sslCLASS_ELEMENT.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<sslCLASS_ELEMENT,8>::_internalRealloc @ 0x82A37B88
// Resize the backing store to `size` elements via the debug reallocator. sslCLASS_ELEMENT is a
// trivially relocatable POD (name id + type byte + union pointer), so this is a bare dlRealloc.
// Element size 12 bytes.
template<>
void dsVECTOR<sslCLASS_ELEMENT, 8>::_internalRealloc(int size)
{
    this->pData = (sslCLASS_ELEMENT *)dlRealloc(this->pData, 12 * size, this->__cl.file, this->__cl.line);
}
