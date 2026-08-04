#include "dsVECTOR.h"
#include "../ssl/sslCLASS_REF.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<sslCLASS_REF,8>::_internalRealloc @ 0x825CA1F0
// sslCLASS_REF is a 4-byte ref-counted handle; trivially relocatable at the raw-storage level
// (its refcount lives on the referenced sslCLASS, not the handle itself), so a bare dlRealloc.
template<>
void dsVECTOR<sslCLASS_REF, 8>::_internalRealloc(int size)
{
    this->pData = (sslCLASS_REF *)dlRealloc(this->pData, sizeof(sslCLASS_REF) * size, this->__cl.file, this->__cl.line);
}
