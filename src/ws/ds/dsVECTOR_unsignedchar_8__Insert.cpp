#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<unsigned char,8>::Insert @ 0x82AC8A18
// Splice `count` bytes from `begin` in at index `pos`.
template<>
void dsVECTOR<unsigned char, 8>::Insert(int pos, const unsigned char *begin, int count)
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
    unsigned char *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    // rebase `begin` if it aliased storage that Realloc just moved
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], this->nElem - pos);
    for (int i = 0; i < count; ++i)
        this->pData[pos + i] = begin[i];

    this->nElem += count;
}
