#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<int,8>::Insert @ 0x823B7460  (fill overload: const int&, count)
// Splice `count` copies of `val` in at index `pos`. Fast path when there is spare capacity (just
// shift the tail and store); otherwise capture `val` by value first (it may alias storage about to
// be reallocated), grow (doubling policy), shift, and store.
template<>
void dsVECTOR<int, 8>::Insert(int pos, const int &val, int count)
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
    int needed = nElem + count;
    if (allocated >= needed) {
        memmove(&this->pData[pos + count], &this->pData[pos], sizeof(int) * (nElem - pos));
        for (int i = 0; i < count; ++i)
            this->pData[pos + i] = val;
        this->nElem += count;
        return;
    }

    int fill = val; // capture before a realloc that could invalidate &val
    if (2 * allocated > needed)
        needed = 2 * allocated;
    if (needed > allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(int) * (this->nElem - pos));
    for (int i = 0; i < count; ++i)
        this->pData[pos + i] = fill;
    this->nElem += count;
}
