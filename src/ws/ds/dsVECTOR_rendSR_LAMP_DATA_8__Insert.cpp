#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — rend-subsystem lamp record; DB-verified as a single 4-byte { unsigned int data } POD.
struct rendSR_LAMP_DATA { unsigned int data; };

// dsVECTOR<rendSR_LAMP_DATA,8>::Insert @ 0x8252E328
// Splice `count` lamp records from `begin` in at index `pos`.
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::Insert(int pos, const rendSR_LAMP_DATA *begin, int count)
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
    rendSR_LAMP_DATA *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos],
            sizeof(rendSR_LAMP_DATA) * (this->nElem - pos));
    for (int i = 0; i < count; ++i)
        this->pData[pos + i] = begin[i];

    this->nElem += count;
}
