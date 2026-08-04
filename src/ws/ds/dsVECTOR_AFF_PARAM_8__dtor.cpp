#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM,8>::~dsVECTOR @ 0x82A75F50
// Destroy every live affix parameter (drop its value then name buffer reference), then free
// the backing element storage.
template<>
dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index) {
        dsAFFIX_BUILDER::AFF_PARAM *param = &this->pData[index];
        dsTSTRING_BUF_HEADER<char> *valueBuffer = param->value.pBuffer;
        if (valueBuffer->refCount-- == 1)
            dlFree(valueBuffer);
        dsTSTRING_BUF_HEADER<char> *nameBuffer = param->name.pBuffer;
        if (--nameBuffer->refCount == 0)
            dlFree(nameBuffer);
    }
    dlFree(this->pData);
}
