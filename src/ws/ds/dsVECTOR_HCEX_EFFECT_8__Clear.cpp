#include "dsVECTOR.h"
#include "../../headers/hcex/HCEX_EFFECT.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<HCEX_EFFECT,8>::Clear @ 0x823DAF50
// Destroy every live effect (drop its name buffer reference and tear down its locations
// vector), then reset the element count to zero.
template<>
void dsVECTOR<HCEX_EFFECT, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        HCEX_EFFECT *effect = &this->pData[index];
        dsTSTRING_BUF_HEADER<char> *nameBuffer = effect->hcexName.pBuffer;
        if (nameBuffer->refCount-- == 1)
            dlFree(nameBuffer);
        effect->locations.~dsVECTOR();
    }
    this->nElem = 0;
}
