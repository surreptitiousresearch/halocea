#include "dsVECTOR.h"

class apCOUNTER; // boundary — ap platform performance counter (opaque; element is a pointer)

// dsVECTOR<apCOUNTER *,8>::PushBack @ 0x82718CA8
// Append `val` to the tail, growing the backing store first when there is no spare capacity,
// and return a reference to the newly stored pointer slot.
template<>
apCOUNTER *&dsVECTOR<apCOUNTER *, 8>::PushBack(apCOUNTER *const &val)
{
    if (this->allocated >= this->nElem + 1) {
        // spare capacity: store straight into the tail slot
        this->pData[this->nElem] = val;
    } else {
        // must grow: save `val` first in case it aliases storage about to be reallocated
        apCOUNTER *saved = val;
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        this->pData[this->nElem] = saved;
    }
    ++this->nElem;
    return this->Back();
}
