#include <string.h>
#include "dsVECTOR.h"
#include "WEAK_PTR.h"
#include "ds_assert_boundary.h"

struct entENTITY; // boundary — ws-engine entity object (weak-referenced element)

// boundary — os interlocked increment of a 32-bit word; returns the new value.
extern "C" int osLockedIncrement(volatile int *dest);

// dsVECTOR<ds::WEAK_PTR<entENTITY>,8>::Insert @ 0x823EE3C0
// Range-insert `count` weak references from `begin` at index `pos`: grow (doubling policy), shift the
// tail up, then copy-construct each new element. Copying a WEAK_PTR shares the handle and bumps its
// refcount.
template<>
void dsVECTOR<ds::WEAK_PTR<entENTITY>, 8>::Insert(
        int pos, const ds::WEAK_PTR<entENTITY> *begin, int count)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || count < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && num >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            382,
            "pos", pos, "nElem", this->nElem, "num", count);
    if (count == 0)
        return;

    int needed = this->nElem + count;
    ds::WEAK_PTR<entENTITY> *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(ds::WEAK_PTR<entENTITY>) * (this->nElem - pos));
    for (int i = 0; i < count; ++i) {
        ds::WEAK_PTR_HANDLE<entENTITY> *handle = begin[i].pHandle;
        this->pData[pos + i].pHandle = handle; // share the handle
        if (handle)
            osLockedIncrement(&handle->refCount);
    }

    this->nElem += count;
}
