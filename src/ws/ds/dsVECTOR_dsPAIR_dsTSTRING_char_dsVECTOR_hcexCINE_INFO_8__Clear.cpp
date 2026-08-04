#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "hcexCINE_INFO.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsVECTOR<hcexCINE_INFO,8>>,8>::Clear @ 0x823B4F80
// Destroy every live listCine pair, then reset the count. Each element's destructor is inlined:
// the nested dsVECTOR<hcexCINE_INFO,8> value (trivially-destructible elements) releases its heap
// buffer directly via dlFree(val.pData), then the string key drops one reference on its shared
// buffer header (freeing at zero).
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, dsVECTOR<hcexCINE_INFO, 8> >, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsPAIR<dsTSTRING<char>, dsVECTOR<hcexCINE_INFO, 8> > *pair = &this->pData[index];
        dlFree(pair->val.pData); // inlined nested dsVECTOR destructor (POD elements)
        dsTSTRING_BUF_HEADER<char> *keyBuffer = pair->key.pBuffer;
        if (--keyBuffer->refCount == 0)
            dlFree(keyBuffer);
    }
    this->nElem = 0;
}
