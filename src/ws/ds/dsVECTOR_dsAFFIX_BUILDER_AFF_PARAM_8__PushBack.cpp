#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM,8>::PushBack @ 0x82A77540
// Append a copy of `val` (a name/value pair of ref-counted strings) and return the new last element.
// Each dsTSTRING member is copied by sharing the source buffer and bumping its refCount. When the
// vector must grow, both source buffers are pre-referenced across the Realloc, then the extra
// reference is dropped after the element is stored (freeing any buffer whose count hits zero).
template<>
dsAFFIX_BUILDER::AFF_PARAM &dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8>::PushBack(
        const dsAFFIX_BUILDER::AFF_PARAM &val)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        dsAFFIX_BUILDER::AFF_PARAM *slot = &this->pData[nElem];
        if (slot) {
            slot->name.pBuffer = val.name.pBuffer;
            ++slot->name.pBuffer->refCount;
            slot->value.pBuffer = val.value.pBuffer;
            ++slot->value.pBuffer->refCount;
        }
    } else {
        dsTSTRING_BUF_HEADER<char> *nameBuf = val.name.pBuffer;
        ++nameBuf->refCount;
        dsTSTRING_BUF_HEADER<char> *valueBuf = val.value.pBuffer;
        ++valueBuf->refCount;

        int allocated = this->allocated;
        int newCapacity = this->nElem + 1;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        dsAFFIX_BUILDER::AFF_PARAM *slot = &this->pData[this->nElem];
        if (slot) {
            slot->name.pBuffer = nameBuf;
            ++nameBuf->refCount;
            slot->value.pBuffer = valueBuf;
            ++valueBuf->refCount;
        }
        // drop the pre-references taken across the Realloc
        if (valueBuf->refCount-- == 1)
            dlFree(valueBuf);
        if (nameBuf->refCount-- == 1)
            dlFree(nameBuf);
    }
    ++this->nElem;
    return this->Back();
}
