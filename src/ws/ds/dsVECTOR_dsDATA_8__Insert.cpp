#include <string.h>
#include "dsVECTOR.h"
#include "dsDATA.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsDATA,8>::Insert @ 0x827AF840
// Range-insert `count` dsDATA values from `begin` at index `pos`: grow (doubling policy), shift the
// tail up, then copy-construct each new element. Each element is placement-initialised (type=null)
// then filled via StoreValue(const dsDATA&), matching the emitted per-element construction.
template<>
void dsVECTOR<dsDATA, 8>::Insert(int pos, const dsDATA *begin, int count)
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
    dsDATA *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(dsDATA) * (this->nElem - pos));
    for (int i = 0; i < count; ++i) {
        dsDATA *slot = &this->pData[pos + i];
        slot->type = nullptr;             // placement-init the empty value
        slot->StoreValue(begin[i]);       // copy-construct from the source dsDATA
    }

    this->nElem += count;
}
