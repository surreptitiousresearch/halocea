#include <string.h>
#include <new>
#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"
#include "ds_assert_boundary.h"

// boundary — global placement copy-construct helper: New<T>(slot, src) === new (slot) T(*src).
#include <new>
template<class T> static T *New(T *slot, const T *src) { return ::new (static_cast<void *>(slot)) T(*src); }

// dsVECTOR<sslCLASS_MAP_ELEM,8>::Insert(pos,val,n) @ 0x825CFE20
// Fill-insert: splice `n` copies of the single record `val` in at index `pos`. When there is
// already spare capacity, shift the tail up (memmove) and copy-construct each new slot directly
// from `val` (name copied by value, cls ref-counted copy). Otherwise copy-construct a local
// temporary from `val` FIRST (in case `val` aliases storage about to be reallocated), grow,
// shift, copy-construct each new slot from the temporary, then destroy the temporary's cls ref.
template<>
void dsVECTOR<sslCLASS_MAP_ELEM, 8>::Insert(int pos, const sslCLASS_MAP_ELEM &val, int n)
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
        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(sslCLASS_MAP_ELEM) * (nElem - pos));
        for (int i = pos; i < pos + n; ++i)
            New<sslCLASS_MAP_ELEM>(&this->pData[i], &val);
        this->nElem += n;
        return;
    }

    sslCLASS_MAP_ELEM temp; // save before Realloc may invalidate `val` if it aliases storage
    temp.name.id = val.name.id;
    new (&temp.cls) sslCLASS_REF(val.cls);

    int allocated = this->allocated;
    int needed = this->nElem + n;
    if (2 * allocated > needed)
        needed = 2 * allocated;
    if (needed > allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + n], &this->pData[pos], sizeof(sslCLASS_MAP_ELEM) * (this->nElem - pos));
    for (int i = pos; i < pos + n; ++i)
        New<sslCLASS_MAP_ELEM>(&this->pData[i], &temp);

    temp.cls.~sslCLASS_REF();
    this->nElem += n;
}
