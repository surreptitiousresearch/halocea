// dsVECTOR<dsPAIR<dsTSTRING<char>,dsVECTOR<hcexCINE_INFO,8>>,8>::~dsVECTOR() @ 0x823B4F10
//
// Destroy the listCine registry. Each live element's dsPAIR destructor is inlined (free the value
// vector's storage; release the key string's shared buffer, freeing it at a zero refcount), then
// the backing store is freed. Reversed per-instantiation.

#include "../headers/hcex/hcexCINE_INFO.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsPAIR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

typedef dsPAIR<dsTSTRING<char>, dsVECTOR<hcexCINE_INFO, 8> > hcex_cine_entry;

template<>
dsVECTOR<hcex_cine_entry, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
    {
        hcex_cine_entry *entry = &this->pData[i];
        dlFree(entry->val.pData);                              // inlined value dsVECTOR dtor
        dsTSTRING_BUF_HEADER<char> *pBuffer = entry->key.pBuffer; // inlined key dsTSTRING dtor
        if (--pBuffer->refCount == 0)
            dlFree(pBuffer);
    }
    dlFree(this->pData);
}
