#include <string.h>
#include <new>
#include "dsVECTOR.h"
#include "../../headers/hcex/HCEX_MODEL_REGION.h"
#include "ds_assert_boundary.h"

// dsVECTOR<HCEX_MODEL_REGION,8>::Insert @ 0x823E43D0
// Range-insert `count` model regions from `begin` at index `pos`: grow (doubling policy), shift the
// tail up, then copy-construct each new element via the region copy-constructor.
template<>
void dsVECTOR<HCEX_MODEL_REGION, 8>::Insert(
        int pos, const HCEX_MODEL_REGION *begin, int count)
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
    HCEX_MODEL_REGION *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(HCEX_MODEL_REGION) * (this->nElem - pos));
    for (int i = 0; i < count; ++i)
        new (&this->pData[pos + i]) HCEX_MODEL_REGION(begin[i]); // placement copy-construct

    this->nElem += count;
}
