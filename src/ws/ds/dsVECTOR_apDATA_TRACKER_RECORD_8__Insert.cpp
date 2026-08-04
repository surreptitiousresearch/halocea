#include <new>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../ap/apDATA_TRACKER.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" void *memcpy(void *dst, const void *src, unsigned int n);  // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc free

// dsVECTOR<apDATA_TRACKER::RECORD,8>::Insert @ 0x82A24F20  (fill overload: const RECORD&, n)
// Splice `n` copies of `val` in at index `pos`. Fast path when there is spare capacity (shift the
// tail up and copy-construct each slot directly from `val`). Otherwise capture `val` into a stack
// temp FIRST (share its name buffer + copy its 30-float array; the temp may alias storage about to
// be reallocated), grow (doubling policy), shift, copy each slot from the temp, then release the
// temp's name-buffer reference. Copy-construction is placement-new of the trivially-copyable-plus-
// refcounted-string RECORD (the decompiler's New<RECORD>).
template<>
void dsVECTOR<apDATA_TRACKER::RECORD, 8>::Insert(
        int pos, const apDATA_TRACKER::RECORD &val, int n)
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
        memmove(&this->pData[pos + n], &this->pData[pos],
                sizeof(apDATA_TRACKER::RECORD) * (nElem - pos));
        for (int i = pos; i < pos + n; ++i)
            new (&this->pData[i]) apDATA_TRACKER::RECORD(val);
        this->nElem += n;
        return;
    }

    // Capture `val` before a realloc that could invalidate &val: share its name buffer + copy values.
    apDATA_TRACKER::RECORD fill;
    fill.name.pBuffer = val.name.pBuffer;
    ++fill.name.pBuffer->refCount;
    memcpy(fill.values, val.values, sizeof(fill.values));

    int needed = this->nElem + n;
    if (2 * this->allocated > needed)
        needed = 2 * this->allocated;
    if (needed > this->allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + n], &this->pData[pos],
            sizeof(apDATA_TRACKER::RECORD) * (this->nElem - pos));
    for (int i = pos; i < pos + n; ++i)
        new (&this->pData[i]) apDATA_TRACKER::RECORD(fill);

    // Release the captured temp's buffer reference (free the header if it was the last owner).
    dsTSTRING_BUF_HEADER<char> *buf = fill.name.pBuffer;
    if (buf->refCount-- == 1)
        dlFree(buf);
    this->nElem += n;
}
