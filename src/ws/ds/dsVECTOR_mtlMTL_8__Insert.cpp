#include <new>
#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../mtl/mtlMTL.h"

// dsVECTOR<mtlMTL,8>::Insert @ 0x82688F40
// Splice `count` materials from `begin` in at index `pos`.
template<>
void dsVECTOR<mtlMTL, 8>::Insert(int pos, const mtlMTL *begin, int count)
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
    mtlMTL *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(mtlMTL) * (this->nElem - pos));
    for (int i = 0; i < count; ++i)
        new (&this->pData[pos + i]) mtlMTL(begin[i]); // copy-construct the inserted element

    this->nElem += count;
}
