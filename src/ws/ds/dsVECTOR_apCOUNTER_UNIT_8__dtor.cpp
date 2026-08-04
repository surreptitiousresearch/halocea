#include "dsVECTOR.h"
#include "../ap/apCOUNTER_UNIT.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apCOUNTER_UNIT,8>::~dsVECTOR @ 0x823D5BA8
// Destroy each live counter by dropping its name buffer's ref-count (freeing at zero), then
// release the backing storage.
template<>
dsVECTOR<apCOUNTER_UNIT, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsTSTRING_BUF_HEADER<char> *nameBuffer = this->pData[index].name.pBuffer;
        if (nameBuffer->refCount-- == 1)
            dlFree(nameBuffer);
    }
    dlFree(this->pData);
}
