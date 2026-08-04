#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<short,8>::Insert @ 0x823E02A0
// Splice `count` shorts from `begin` in at index `pos`.
template<>
void dsVECTOR<short, 8>::Insert(int pos, const short *begin, int count)
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
    short *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(short) * (this->nElem - pos));
    for (int i = 0; i < count; ++i)
        this->pData[pos + i] = begin[i];

    this->nElem += count;
}
