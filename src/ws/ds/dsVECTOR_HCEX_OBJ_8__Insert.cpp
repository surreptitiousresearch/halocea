#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_OBJ.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc free

// boundary — HCEX_OBJ copy-construct (id/pInst/modelIdx/flags bit-copied; followers deep-copied;
// followersOfs storage duplicated). Modeled as a free helper because HCEX_OBJ.h is a plain C struct.
#include <new>
static void CopyConstructHcexObj(HCEX_OBJ *dst, const HCEX_OBJ *src) { ::new (static_cast<void *>(dst)) HCEX_OBJ(*src); }
// boundary — the inlined dsVECTOR<ds::WEAK_PTR<entENTITY>,8> destructor (drops each weak ref and
// releases the backing storage), as in dsVECTOR<HCEX_OBJ,8>::Erase.
void DestroyFollowersVector(dsVECTOR_followers *followers); // boundary (non-static: defined elsewhere)

// dsVECTOR<HCEX_OBJ,8>::Insert @ 0x823EEE90  (fill overload: const HCEX_OBJ&, n)
// Splice `n` copies of `val` in at index `pos`. Fast path when there is spare capacity (shift the
// tail up and copy-construct each slot directly from `val`). Otherwise capture `val` into a stack
// temp FIRST (it may alias storage about to be reallocated), grow (doubling policy), shift, copy
// each slot from the temp, then tear the temp down (free followersOfs storage + destroy followers).
template<>
void dsVECTOR<HCEX_OBJ, 8>::Insert(int pos, const HCEX_OBJ &val, int n)
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
        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(HCEX_OBJ) * (nElem - pos));
        for (int i = pos; i < pos + n; ++i)
            CopyConstructHcexObj(&this->pData[i], &val);
        this->nElem += n;
        return;
    }

    HCEX_OBJ fill;                       // capture before a realloc that could invalidate &val
    CopyConstructHcexObj(&fill, &val);
    int needed = this->nElem + n;
    if (2 * this->allocated > needed)
        needed = 2 * this->allocated;
    if (needed > this->allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + n], &this->pData[pos], sizeof(HCEX_OBJ) * (this->nElem - pos));
    for (int i = pos; i < pos + n; ++i)
        CopyConstructHcexObj(&this->pData[i], &fill);

    dlFree(fill.followersOfs.pData);
    DestroyFollowersVector(&fill.followers);
    this->nElem += n;
}
