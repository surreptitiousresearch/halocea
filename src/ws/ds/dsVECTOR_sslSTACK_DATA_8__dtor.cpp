#include "dsVECTOR.h"
#include "../ssl/sslSTACK_DATA.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<sslSTACK_DATA,8>::~dsVECTOR @ 0x825CA380
// Destroy each live element via the real sslSTACK_DATA destructor (releases any held object/class
// ref-count per its TYPE), then free the backing storage.
template<>
dsVECTOR<sslSTACK_DATA, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].~sslSTACK_DATA();
    dlFree(this->pData);
}
