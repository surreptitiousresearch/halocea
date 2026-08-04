#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

// dsVECTOR<dscBRAND*,8>::Insert @ 0x82711760
// Fill overload: splice `n` copies of the single pointer `val` in at index `pos`. Grow (doubling
// policy) and shift the tail up, then store the pointer into each opened slot. The element is a
// raw pointer (trivially relocatable), so the shift is a memmove and no per-element construction
// runs.
template<>
void dsVECTOR<dscBRAND *, 8>::Insert(int pos, dscBRAND *const &val, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || n < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && n >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            338,
            "pos", pos, "nElem", this->nElem, "n", n);
    if (n == 0)
        return;

    if (this->allocated >= this->nElem + n) {
        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dscBRAND *) * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + n; ++slotIdx) {
            dscBRAND **slot = &this->pData[slotIdx];
            if (slot)
                *slot = val;
        }
    } else {
        dscBRAND *valCopy = val; // cache: `val` may alias storage that Realloc frees
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + n)
            newCapacity = this->nElem + n;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dscBRAND *) * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + n; ++slotIdx) {
            dscBRAND **slot = &this->pData[slotIdx];
            if (slot)
                *slot = valCopy;
        }
    }
    this->nElem += n;
}
