#include <string.h>
#include "dsVECTOR.h"
#include "../ssl/sslBYTECODE_INSTRUCTION.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslBYTECODE_INSTRUCTION,8>::Insert(pos,begin,count) @ 0x8308A080
// Range-insert `count` instructions from `begin` at index `pos`: grow to nElem+count (doubling
// policy) if needed, shift the tail up by `count` (memmove), then copy each new slot (trivially
// relocatable POD). Adjusts `begin` if it aliased storage that was reallocated.
template<>
void dsVECTOR<sslBYTECODE_INSTRUCTION, 8>::Insert(int pos, const sslBYTECODE_INSTRUCTION *begin, int count)
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
    sslBYTECODE_INSTRUCTION *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(sslBYTECODE_INSTRUCTION) * (this->nElem - pos));
    for (int i = 0; i < count; ++i)
        this->pData[pos + i] = begin[i];

    this->nElem += count;
}
