#include "dsVECTOR.h"

class strmVID_MEM_BLOCK; // boundary — strm streaming video-memory block (opaque; element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK *,8>::PushBack @ 0x8269BB58
// Append `val` to the tail, growing the backing store first when there is no spare capacity,
// and return a reference to the newly stored pointer slot.
template<>
strmVID_MEM_BLOCK *&dsVECTOR<strmVID_MEM_BLOCK *, 8>::PushBack(strmVID_MEM_BLOCK *const &val)
{
    if (this->allocated >= this->nElem + 1) {
        // spare capacity: store straight into the tail slot
        this->pData[this->nElem] = val;
    } else {
        // must grow: save `val` first in case it aliases storage about to be reallocated
        strmVID_MEM_BLOCK *saved = val;
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
