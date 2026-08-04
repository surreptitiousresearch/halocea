#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::PushBack @ 0x825B4D40
// Append a copy of `val`. If spare capacity exists, construct the copy directly into the next slot.
// Otherwise stage the copy in a temporary, grow the backing store (doubling, but at least +1),
// move the staged value into the new tail slot, and drop the temporary. Returns the new last slot.
template<>
dsDATA &dsVECTOR<dsDATA, 8>::PushBack(const dsDATA &val)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        dsDATA *slot = &this->pData[nElem];
        if (slot) {
            slot->type = nullptr;
            slot->StoreValue(val);
        }
    } else {
        dsDATA staged;
        staged.type = nullptr;
        staged.StoreValue(val);

        int allocated = this->allocated;
        int newCapacity = 2 * allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        dsDATA *slot = &this->pData[this->nElem];
        if (slot) {
            slot->type = nullptr;
            slot->StoreValue(staged);
        }
        if (staged.type)
            staged.type->Destroy((unsigned int *)&staged);
    }
    ++this->nElem;
    return this->Back();
}
