#include "dsVECTOR.h"
#include "../../headers/hcex/HCEX_MODEL_REGION.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<HCEX_MODEL_REGION,8>::Clear @ 0x823D78B8
// Destroy every live region (drop its name buffer reference, tear down its objNames vector,
// free its objIds storage), then reset the element count to zero.
template<>
void dsVECTOR<HCEX_MODEL_REGION, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        HCEX_MODEL_REGION *region = &this->pData[index];
        dsTSTRING_BUF_HEADER<char> *nameBuffer = region->name.pBuffer;
        if (nameBuffer->refCount-- == 1)
            dlFree(nameBuffer);
        region->objNames.~dsVECTOR();
        dlFree(region->objIds.pData);
    }
    this->nElem = 0;
}
