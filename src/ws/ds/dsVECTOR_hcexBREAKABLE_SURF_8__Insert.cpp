#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// hcex bridge breakable-surface record; 100 bytes per the DB. Its copy constructor and destructor
// are genuine non-trivial functions in the binary (the embedded scnINST_CREATE_DATA::cd owns
// string/affix resources) but are not reversed in this batch — declared as boundaries, matching
// the sibling dsVECTOR_hcexBREAKABLE_SURF_8__ShrinkResize.cpp convention.
struct hcexBREAKABLE_SURF {
    hcexBREAKABLE_SURF(const hcexBREAKABLE_SURF &other); // boundary — copy ctor
    ~hcexBREAKABLE_SURF();                               // boundary — dtor (tears down cd)
};

// boundary — global placement copy-construct helper: New<T>(slot, src) === new (slot) T(*src).
#include <new>
template<class T> static T *New(T *slot, const T *src) { return ::new (static_cast<void *>(slot)) T(*src); }

// dsVECTOR<hcexBREAKABLE_SURF,8>::Insert(pos,val,n) @ 0x823B2450
// Fill-insert: splice `n` copies of the single record `val` in at index `pos`. When there is
// already spare capacity, shift the tail up (memmove) and copy-construct each new slot directly
// from `val`. Otherwise copy-construct a local temporary from `val` FIRST (in case `val` aliases
// storage about to be reallocated), grow, shift, copy-construct each new slot from the temporary,
// then destroy the temporary.
template<>
void dsVECTOR<hcexBREAKABLE_SURF, 8>::Insert(int pos, const hcexBREAKABLE_SURF &val, int n)
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
        memmove(&this->pData[pos + n], &this->pData[pos], 100 * (nElem - pos));
        for (int i = 0; i < n; ++i)
            New<hcexBREAKABLE_SURF>(&this->pData[pos + i], &val);
        this->nElem += n;
        return;
    }

    hcexBREAKABLE_SURF temp(val); // save before Realloc may invalidate `val` if it aliases storage
    int allocated = this->allocated;
    int needed = this->nElem + n;
    if (2 * allocated > needed)
        needed = 2 * allocated;
    if (needed > allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + n], &this->pData[pos], 100 * (this->nElem - pos));
    for (int i = 0; i < n; ++i)
        New<hcexBREAKABLE_SURF>(&this->pData[pos + i], &temp);

    temp.~hcexBREAKABLE_SURF();
    this->nElem += n;
}
