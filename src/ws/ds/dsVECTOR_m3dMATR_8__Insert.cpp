#include <string.h>
#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<m3dMATR,8>::Insert @ 0x823EE540
// Splice `count` 64-byte matrices from `begin` in at index `pos` (elements are trivially copied).
template<>
void dsVECTOR<m3dMATR, 8>::Insert(int pos, const m3dMATR *begin, int count)
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
    m3dMATR *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(m3dMATR) * (this->nElem - pos));
    for (int i = 0; i < count; ++i)
        memcpy(&this->pData[pos + i], &begin[i], sizeof(m3dMATR)); // raw element copy (as emitted)

    this->nElem += count;
}
