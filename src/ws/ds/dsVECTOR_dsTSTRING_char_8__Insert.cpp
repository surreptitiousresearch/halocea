#include <string.h>
#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

// dsVECTOR<dsTSTRING<char>,8>::Insert @ 0x823DB378  (fill overload: const dsTSTRING<char>&, n)
// Splice `n` copies of the string `val` in at index `pos`. Fast path when there is spare capacity:
// shift the tail up and share `val`'s buffer into each new slot (refcount bump per copy). Otherwise
// pin `val`'s buffer with an extra reference FIRST (it may alias storage about to be reallocated),
// grow (doubling policy), shift, share the pinned buffer into each slot, then drop the extra
// reference (freeing the header if it was the last owner).
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Insert(int pos, const dsTSTRING<char> &val, int n)
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
        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dsTSTRING<char>) * (nElem - pos));
        for (int i = pos; i < pos + n; ++i) {
            dsTSTRING<char> *slot = &this->pData[i];
            slot->pBuffer = nullptr;              // placement-init the slot
            dsTSTRING_BUF_HEADER<char> *buf = val.pBuffer;
            slot->pBuffer = buf;                  // share the source buffer header
            ++buf->refCount;
        }
        this->nElem += n;
        return;
    }

    // Pin the source buffer before a realloc that could invalidate `val`.
    dsTSTRING_BUF_HEADER<char> *pinned = val.pBuffer;
    ++pinned->refCount;

    int needed = this->nElem + n;
    if (2 * this->allocated > needed)
        needed = 2 * this->allocated;
    if (needed > this->allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dsTSTRING<char>) * (this->nElem - pos));
    for (int i = pos; i < pos + n; ++i) {
        this->pData[i].pBuffer = pinned;          // share the pinned buffer header
        ++pinned->refCount;
    }

    // Drop the extra pin reference (free the header if it was the last owner).
    if (pinned->refCount-- == 1)
        dlFree(pinned);
    this->nElem += n;
}
