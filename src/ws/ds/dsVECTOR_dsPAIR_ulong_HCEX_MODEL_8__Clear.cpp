#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING_BUF_HEADER.h"
#include "../../hcex/HCEX_MODEL.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<unsigned long,HCEX_MODEL>,8>::Clear @ 0x823DFDF8
// Destroy every live pair's HCEX_MODEL value, then reset the count. The value destructor is
// inlined in the original: tear down its region/object-name vectors, free its trivially-typed
// node-index storage directly, then drop the two ref-counted string buffers.
template<>
void dsVECTOR<dsPAIR<unsigned long, HCEX_MODEL>, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        HCEX_MODEL &model = this->pData[index].val;
        model.regions.~dsVECTOR<HCEX_MODEL_REGION, 8>();
        dlFree(model.nodeIndices.pData); // dsVECTOR<short,8> dtor: element is trivial, just free storage
        model.objNames.~dsVECTOR<dsTSTRING<char>, 8>();
        dsTSTRING_BUF_HEADER<char> *hcexNameBuffer = model.hcexName.pBuffer;
        if (hcexNameBuffer->refCount-- == 1)
            dlFree(hcexNameBuffer);
        dsTSTRING_BUF_HEADER<char> *haloNameBuffer = model.haloName.pBuffer;
        if (haloNameBuffer->refCount-- == 1)
            dlFree(haloNameBuffer);
    }
    this->nElem = 0;
}
