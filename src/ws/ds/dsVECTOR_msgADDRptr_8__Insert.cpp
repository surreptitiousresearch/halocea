#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem); only a pointer is handled

// dsVECTOR<msgADDR*,8>::Insert @ 0x8255A2F0
// Fill overload: splice `count` copies of the single pointer `val` in at index `pos`. Grow (doubling
// policy) and shift the tail up, then store the pointer into each opened slot. The element is a raw
// pointer (trivially relocatable), so the shift is a memmove and no per-element construction runs.
template<>
void dsVECTOR<msgADDR *, 8>::Insert(int pos, msgADDR *const &val, int count)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || count < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && n >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            338,
            "pos", pos, "nElem", this->nElem, "n", count);
    if (count == 0)
        return;

    if (this->allocated >= this->nElem + count) {
        memmove(&this->pData[pos + count], &this->pData[pos], sizeof(msgADDR *) * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + count; ++slotIdx) {
            msgADDR **slot = &this->pData[slotIdx];
            if (slot)
                *slot = val;
        }
    } else {
        msgADDR *valCopy = val; // cache: `val` may alias storage that Realloc frees
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + count)
            newCapacity = this->nElem + count;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        memmove(&this->pData[pos + count], &this->pData[pos], sizeof(msgADDR *) * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + count; ++slotIdx) {
            msgADDR **slot = &this->pData[slotIdx];
            if (slot)
                *slot = valCopy;
        }
    }
    this->nElem += count;
}
