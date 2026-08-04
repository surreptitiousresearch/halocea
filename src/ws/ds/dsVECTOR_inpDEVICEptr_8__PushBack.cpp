#include "dsVECTOR.h"

struct inpDEVICE; // boundary — ws-engine inp: physical input device (element is a pointer)

// dsVECTOR<inpDEVICE*,8>::PushBack @ 0x826418A0
// Append `val` to the tail, growing the backing store first when there is no spare capacity,
// and return a reference to the newly stored pointer slot.
template<>
inpDEVICE *&dsVECTOR<inpDEVICE *, 8>::PushBack(inpDEVICE *const &val)
{
    if (this->allocated >= this->nElem + 1) {
        // spare capacity: store straight into the tail slot
        this->pData[this->nElem] = val;
    } else {
        // must grow: save `val` first in case it aliases storage about to be reallocated
        inpDEVICE *saved = val;
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
