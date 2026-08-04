#include <string.h>
#include "dsVECTOR.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsSTRID,8>::Insert(pos,val,n) @ 0x8252DE38
// Fill-insert: splice `n` copies of the single element `val` in at index `pos` (grow-and-shift,
// then assign each new slot's id). Saves `val`'s id before a possible Realloc in case `val`
// aliases storage about to be reallocated.
template<>
void dsVECTOR<dsSTRID, 8>::Insert(int pos, const dsSTRID &val, int n)
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
    int allocated = this->allocated;
    if (allocated >= nElem + n) {
        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dsSTRID) * (nElem - pos));
        for (int i = 0; i < n; ++i)
            this->pData[pos + i].id = val.id;
        this->nElem += n;
        return;
    }

    const char *id = val.id; // save before Realloc may invalidate `val` if it aliases storage
    int needed = nElem + n;
    if (2 * allocated > needed)
        needed = 2 * allocated;
    if (needed > allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dsSTRID) * (this->nElem - pos));
    for (int i = 0; i < n; ++i)
        this->pData[pos + i].id = id;

    this->nElem += n;
}
