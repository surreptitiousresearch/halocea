#include "dsVECTOR.h"
#include "../ssl/sslCLASS_REF.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<sslCLASS_REF,8>::~dsVECTOR @ 0x825CA560
// Destroy each live element via the real sslCLASS_REF destructor (drops its class ref-count),
// then free the backing storage.
template<>
dsVECTOR<sslCLASS_REF, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].~sslCLASS_REF();
    dlFree(this->pData);
}
