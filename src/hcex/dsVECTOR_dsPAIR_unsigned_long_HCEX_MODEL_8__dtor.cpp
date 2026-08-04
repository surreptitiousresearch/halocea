// dsVECTOR<dsPAIR<unsigned long,HCEX_MODEL>,8>::~dsVECTOR() @ 0x823DFD6C
//
// Destroy the global hcexModels table. Per element the compiler inlined the dsPAIR destructor,
// whose value is an HCEX_MODEL: destroy regions, free nodeIndices' backing store, destroy objNames,
// then release hcexName's and haloName's shared buffers (freeing each at a zero refcount). The
// unsigned-long key is trivially destructible. Finally free the backing store. Reversed
// per-instantiation.

#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsPAIR.h"
#include "../headers/hcex/HCEX_MODEL.h"
#include "../headers/ws/ds/dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

typedef dsPAIR<unsigned long, HCEX_MODEL> hcex_model_pair;

template<>
dsVECTOR<hcex_model_pair, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
    {
        HCEX_MODEL *pVal = &this->pData[i].val;
        pVal->regions.~dsVECTOR();
        dlFree(pVal->nodeIndices.pData);
        pVal->objNames.~dsVECTOR();
        dsTSTRING_BUF_HEADER<char> *hcexBuffer = pVal->hcexName.pBuffer;
        if (hcexBuffer->refCount-- == 1)
            dlFree(hcexBuffer);
        dsTSTRING_BUF_HEADER<char> *haloBuffer = pVal->haloName.pBuffer;
        if (haloBuffer->refCount-- == 1)
            dlFree(haloBuffer);
    }
    dlFree(this->pData);
}
