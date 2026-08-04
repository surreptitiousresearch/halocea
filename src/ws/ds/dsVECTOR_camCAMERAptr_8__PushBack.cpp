#include "dsVECTOR.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)

// dsVECTOR<camCAMERA*,8>::PushBack @ 0x82704AF0
// Append `val` to the tail, growing the backing store first when there is no spare capacity,
// and return a reference to the newly stored pointer slot.
template<>
camCAMERA *&dsVECTOR<camCAMERA *, 8>::PushBack(camCAMERA *const &val)
{
    if (this->allocated >= this->nElem + 1) {
        // spare capacity: store straight into the tail slot
        this->pData[this->nElem] = val;
    } else {
        // must grow: save `val` first in case it aliases storage about to be reallocated
        camCAMERA *saved = val;
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
