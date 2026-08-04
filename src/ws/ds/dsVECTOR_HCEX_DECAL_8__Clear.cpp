#include "dsVECTOR.h"
#include "../../headers/hcex/HCEX_DECAL.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<HCEX_DECAL,8>::Clear @ 0x823D5D10
// Drop each live decal's name-buffer reference (freeing at zero), then reset the element count.
// (HCEX_DECAL's `pos` is a trivial float vector, so only `name`'s buffer needs releasing.)
template<>
void dsVECTOR<HCEX_DECAL, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsTSTRING_BUF_HEADER<char> *nameBuffer = this->pData[index].name.pBuffer;
        if (nameBuffer->refCount-- == 1)
            dlFree(nameBuffer);
    }
    this->nElem = 0;
}
