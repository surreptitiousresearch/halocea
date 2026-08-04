#include <string.h>
#include "dsVECTOR.h"
#include "dsDATA.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsDATA,8>::Insert (fill overload: pos, const dsDATA&, count) @ 0x827AF9B8
// Splice `n` copies of `val` in at index `pos`. Fast path when there is spare capacity: shift the
// tail up, then copy-construct each new slot directly from `val` (placement-init type=null, then
// StoreValue). Otherwise capture `val` into a stack scratch FIRST (val may alias storage about to
// be reallocated/moved by the grow), grow (doubling policy), shift, copy each slot from the
// scratch, then release the scratch.
template<>
void dsVECTOR<dsDATA, 8>::Insert(int pos, const dsDATA &val, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || n < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && n >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            338,
            "pos", pos, "nElem", this->nElem, "n", n);
    if (n == 0)
        return;

    int nElem = this->nElem;
    if (this->allocated >= nElem + n) {
        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dsDATA) * (nElem - pos));
        for (int i = pos; i < pos + n; ++i) {
            dsDATA *slot = &this->pData[i];
            slot->type = nullptr;
            slot->StoreValue(val);
        }
        this->nElem += n;
        return;
    }

    dsDATA staged;
    staged.type = nullptr;
    staged.StoreValue(val);

    int needed = this->nElem + n;
    if (2 * this->allocated > needed)
        needed = 2 * this->allocated;
    if (needed > this->allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dsDATA) * (this->nElem - pos));
    for (int i = pos; i < pos + n; ++i) {
        dsDATA *slot = &this->pData[i];
        slot->type = nullptr;
        slot->StoreValue(staged);
    }

    if (staged.type) {
        staged.type->Destroy((unsigned int *)&staged);
        this->nElem += n;
        return;
    }
    this->nElem += n;
}
