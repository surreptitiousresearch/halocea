#include <string.h>
#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM,8>::Insert @ 0x82A773A0
// Range-insert `count` affix params from `begin` at index `pos`: grow (doubling policy), shift the
// tail up, then copy-construct each new element. An AFF_PARAM copy shares both member string buffers
// (name, value) and bumps each header refcount — emitted inline rather than via a ctor call.
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8>::Insert(
        int pos, const dsAFFIX_BUILDER::AFF_PARAM *begin, int count)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || count < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && num >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            382,
            "pos", pos, "nElem", this->nElem, "num", count);
    if (count == 0)
        return;

    int needed = this->nElem + count;
    dsAFFIX_BUILDER::AFF_PARAM *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(dsAFFIX_BUILDER::AFF_PARAM) * (this->nElem - pos));
    for (int i = 0; i < count; ++i) {
        dsAFFIX_BUILDER::AFF_PARAM *slot = &this->pData[pos + i];

        slot->name.pBuffer = nullptr;                        // placement-init the name string
        dsTSTRING_BUF_HEADER<char> *nameBuf = begin[i].name.pBuffer;
        slot->name.pBuffer = nameBuf;                        // share the name buffer
        ++nameBuf->refCount;

        slot->value.pBuffer = nullptr;                       // placement-init the value string
        dsTSTRING_BUF_HEADER<char> *valueBuf = begin[i].value.pBuffer;
        slot->value.pBuffer = valueBuf;                      // share the value buffer
        ++valueBuf->refCount;
    }

    this->nElem += count;
}
