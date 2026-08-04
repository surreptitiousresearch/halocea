#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::Clear @ 0x82A75ED0
// Drop a reference on every live pair's key and val string buffers (freeing at zero), then reset
// the count. Value is released before key, matching the decompiled order.
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *pair = &this->pData[index];
        dsTSTRING_BUF_HEADER<char> *valBuffer = pair->val.pBuffer;
        if (valBuffer->refCount-- == 1)
            dlFree(valBuffer);
        dsTSTRING_BUF_HEADER<char> *keyBuffer = pair->key.pBuffer;
        if (keyBuffer->refCount-- == 1)
            dlFree(keyBuffer);
    }
    this->nElem = 0;
}
