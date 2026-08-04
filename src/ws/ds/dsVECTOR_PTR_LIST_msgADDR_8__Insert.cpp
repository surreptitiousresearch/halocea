#include <string.h>
#include "dsVECTOR.h"
#include "PTR_LIST.h"
#include "ds_assert_boundary.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::Insert @ 0x8255AFB0
// Fill overload: splice `count` copies of the list head record `val` (pHead/pTail/length) in at
// index `pos`. Grow (doubling policy) and shift the tail up (memmove), then copy the three fields
// into each opened slot. The record owns no external storage, so the copy is a plain field copy.
template<>
void dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::Insert(
        int pos, const ds::PTR_LIST<msgADDR> &val, int count)
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
        memmove(&this->pData[pos + count], &this->pData[pos],
                sizeof(ds::PTR_LIST<msgADDR>) * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + count; ++slotIdx) {
            ds::PTR_LIST<msgADDR> *slot = &this->pData[slotIdx];
            if (slot) {
                slot->pHead = val.pHead;
                slot->pTail = val.pTail;
                slot->length = val.length;
            }
        }
    } else {
        // Cache the fields: `val` may alias storage that Realloc frees.
        msgADDR *valHead = val.pHead;
        msgADDR *valTail = val.pTail;
        int valLength = val.length;
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + count)
            newCapacity = this->nElem + count;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        memmove(&this->pData[pos + count], &this->pData[pos],
                sizeof(ds::PTR_LIST<msgADDR>) * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + count; ++slotIdx) {
            ds::PTR_LIST<msgADDR> *slot = &this->pData[slotIdx];
            if (slot) {
                slot->pHead = valHead;
                slot->pTail = valTail;
                slot->length = valLength;
            }
        }
    }
    this->nElem += count;
}
