#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM,8>::Clear @ 0x82A75CD8
// Destroy every live affix parameter (drop its value buffer reference, then its name buffer
// reference), then reset the element count to zero.
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8>::Clear()
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
    this->nElem = 0;
}
