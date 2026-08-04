#include <new>
#include <string.h>
#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"
#include "ds_assert_boundary.h"

// dsVECTOR<mtlMTL,8>::Insert(int,const mtlMTL&,int) @ 0x82C2CD38  (fill-insert overload)
// Splice `n` copies of the single element `val` in at index `pos`: grow storage (doubling policy)
// if needed, shift the tail up by `n` slots, then copy-construct `n` new elements from `val`.
//
// DEVIATION NOTE: same aliasing precaution as PushBack — when growth is required, `val` is copied
// into raw stack storage BEFORE Realloc runs (Realloc's dlRealloc may move/free the storage `val`
// points into), then the fill loop copy-constructs from that stack copy instead of `val` directly.
// Modeled as placement-new into a raw byte buffer + explicit destructor call, matching the
// disassembly's construct/destroy pair rather than a normal C++ automatic local.
// NOTE: the decompiler emitted extra phantom int params (a5..a18) from the STRONG_ASSERT2 varargs
// helper folded into the caller's frame — only `pos`, `val`, and `n` are real parameters.
template<>
void dsVECTOR<mtlMTL, 8>::Insert(int pos, const mtlMTL &val, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || n < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && n >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            338,
            "pos", pos,
            "nElem", this->nElem,
            "n", n);

    if (n == 0)
        return;

    int liveCount = this->nElem;
    if (this->allocated >= liveCount + n) {
        memmove(&this->pData[pos + n], &this->pData[pos], 60 * (liveCount - pos));
        for (int index = pos; index < pos + n; ++index)
            if (&this->pData[index])
                new (&this->pData[index]) mtlMTL(val); // copy-construct the inserted element
        this->nElem += n;
    } else {
        alignas(mtlMTL) unsigned char deferredValStorage[sizeof(mtlMTL)];
        mtlMTL *deferredVal = reinterpret_cast<mtlMTL *>(deferredValStorage);
        new (deferredVal) mtlMTL(val); // stack copy, survives the reallocation below

        int capacity = this->allocated;
        int needed = this->nElem + n;
        if (2 * capacity > needed)
            needed = 2 * capacity;
        if (needed > capacity)
            this->Realloc(needed);

        memmove(&this->pData[pos + n], &this->pData[pos], 60 * (this->nElem - pos));
        for (int index = pos; index < pos + n; ++index)
            if (&this->pData[index])
                new (&this->pData[index]) mtlMTL(*deferredVal);
        deferredVal->~mtlMTL();
        this->nElem += n;
    }
}
