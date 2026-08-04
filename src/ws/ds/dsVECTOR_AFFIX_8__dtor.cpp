#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::~dsVECTOR @ 0x82A764D8
// Destroy each live affix (tear down its parameter vector and drop its name buffer reference),
// then free the backing element storage.
template<>
dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsAFFIX_BUILDER::AFFIX *affix = &this->pData[index];
        affix->params.~dsVECTOR();
        dsTSTRING_BUF_HEADER<char> *nameBuffer = affix->name.pBuffer;
        if (--nameBuffer->refCount == 0)
            dlFree(nameBuffer);
    }
    dlFree(this->pData);
}
