// dsPAIR<dsTSTRING<char>,dsVECTOR<hcexCINE_INFO,8>>::~dsPAIR() @ 0x823B3ED8
//
// Destroy one listCine entry (cinematic name -> per-difficulty (actor,mask) table). The compiler
// inlined both member destructors into the pair's: the value dsVECTOR<hcexCINE_INFO,8> is freed by
// a bare dlFree of its storage (hcexCINE_INFO is trivially destructible, so no per-element loop),
// and the key dsTSTRING<char> releases its shared buffer, freeing it at a zero refcount. Reversed
// per-instantiation.

#include "../headers/hcex/hcexCINE_INFO.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsPAIR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

typedef dsPAIR<dsTSTRING<char>, dsVECTOR<hcexCINE_INFO, 8> > hcex_cine_entry;

template<>
hcex_cine_entry::~dsPAIR()
{
    dlFree(this->val.pData);                       // inlined dsVECTOR<hcexCINE_INFO,8>::~dsVECTOR
    dsTSTRING_BUF_HEADER<char> *pBuffer = this->key.pBuffer; // inlined dsTSTRING<char>::~dsTSTRING
    if (--pBuffer->refCount == 0)
        dlFree(pBuffer);
}
