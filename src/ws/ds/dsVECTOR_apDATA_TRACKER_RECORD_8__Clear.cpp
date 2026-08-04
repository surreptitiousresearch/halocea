#include "dsVECTOR.h"
#include "../ap/apDATA_TRACKER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apDATA_TRACKER::RECORD,8>::Clear @ 0x82A246B8
// Destroy every live element's name (drop its string buffer reference, freeing at zero) and
// reset the count. `values[30]` is a plain float array (nothing to destroy).
template<>
void dsVECTOR<apDATA_TRACKER::RECORD, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsTSTRING_BUF_HEADER<char> *nameBuffer = this->pData[index].name.pBuffer;
        if (nameBuffer->refCount-- == 1)
            dlFree(nameBuffer);
    }
    this->nElem = 0;
}
