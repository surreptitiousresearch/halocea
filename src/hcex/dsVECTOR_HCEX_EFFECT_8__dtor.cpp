// dsVECTOR<HCEX_EFFECT,8>::~dsVECTOR() @ 0x823DAEE0
//
// Destroy the effect table. Per element, the compiler inlined the HCEX_EFFECT destructor: release
// hcexName's shared buffer (freeing at a zero refcount), then destroy the locations vector. Finally
// free the backing store. Reversed per-instantiation.

#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/HCEX_EFFECT.h"
#include "../headers/ws/ds/dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

template<>
dsVECTOR<HCEX_EFFECT, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
    {
        HCEX_EFFECT *pElem = &this->pData[i];
        dsTSTRING_BUF_HEADER<char> *pBuffer = pElem->hcexName.pBuffer; // inlined dsTSTRING release
        if (pBuffer->refCount-- == 1)
            dlFree(pBuffer);
        pElem->locations.~dsVECTOR();
    }
    dlFree(this->pData);
}
