#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<unsigned char,8>::Insert @ 0x82AC8A18  (range overload: const unsigned char*, count)
// Splice `count` bytes from `begin` in at index `pos`: grow to nElem+count (doubling policy) if
// needed, shift the tail up by `count` (memmove), then copy the range in — re-basing `begin` first
// if it aliased the old storage that Realloc just replaced.
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

    int allocated = this->allocated;
    int needed = this->nElem + count;
    unsigned char *oldData = this->pData;
    if (allocated < needed) {
        if (2 * allocated > needed)
            needed = 2 * allocated;
        if (needed > allocated)
            this->Realloc(needed);
    }

    // Deviation from decompiler: reconstructed as a plain pointer re-base rather than the raw
    // `begin += this->pData - pData` idiom it emitted — same effect, only meaningful when Realloc
    // actually moved the storage and `begin` pointed into the old buffer.
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], this->nElem - pos);
    for (int i = 0; i < count; ++i)
        this->pData[pos + i] = begin[i];

    this->nElem += count;
}
