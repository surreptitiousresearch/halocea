#include <string.h>
#include "dsVECTOR.h"
#include "../ssl/sslCLASS_NAME_TO_IDX.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslCLASS::NAME_TO_IDX,8>::Insert(pos,begin,count) @ 0x82A3B818
// Range-insert `count` elements from `begin` at index `pos`: grow to nElem+count (doubling
// policy) if needed, shift the tail up by `count` (memmove), then copy each new slot (trivially
// relocatable). CAVEAT: the decompile fuses (count,begin) into one `__PAIR64__` local
// (reg-alloc artifact on this 32-bit target) — reconstructed here as the plain two-argument form
// matching every sibling dsVECTOR<T,8>::Insert(pos,begin,count) overload.
template<>
void dsVECTOR<ssl_class::NAME_TO_IDX, 8>::Insert(int pos, const ssl_class::NAME_TO_IDX *begin, int count)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || count < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && n >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            338,
            "pos", pos, "nElem", this->nElem, "n", count);
    if (count == 0)
        return;

    int nElem = this->nElem;
    int allocated = this->allocated;
    if (allocated >= nElem + count) {
        memmove(&this->pData[pos + count], &this->pData[pos], sizeof(ssl_class::NAME_TO_IDX) * (nElem - pos));
        for (int i = pos; i < pos + count; ++i)
            this->pData[i] = *begin;
        this->nElem += count;
        return;
    }

    ssl_class::NAME_TO_IDX saved = *begin; // save before Realloc may invalidate `begin`
    int needed = nElem + count;
    if (2 * allocated > needed)
        needed = 2 * allocated;
    if (needed > allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(ssl_class::NAME_TO_IDX) * (this->nElem - pos));
    for (int i = pos; i < pos + count; ++i)
        this->pData[i] = saved;

    this->nElem += count;
}
