#include <string.h>
#include <new>
#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::Insert @ 0x82A77BC0
// Range-insert `count` affixes from `begin` at index `pos`: grow (doubling policy), shift the tail
// up, then copy-construct each new element via the AFFIX copy-constructor.
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::Insert(
        int pos, const dsAFFIX_BUILDER::AFFIX *begin, int count)
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
    dsAFFIX_BUILDER::AFFIX *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(dsAFFIX_BUILDER::AFFIX) * (this->nElem - pos));
    for (int i = 0; i < count; ++i)
        new (&this->pData[pos + i]) dsAFFIX_BUILDER::AFFIX(begin[i]); // placement copy-construct

    this->nElem += count;
}
