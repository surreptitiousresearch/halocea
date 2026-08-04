#include "dsVECTOR.h"

class sslOBJECT; // boundary — ssl scripting engine object (opaque; element is a pointer)

// dsVECTOR<sslOBJECT *,8>::PushBack @ 0x82ADA3D0
// Append `val` to the tail, growing the backing store first when there is no spare capacity,
// and return a reference to the newly stored pointer slot.
template<>
sslOBJECT *&dsVECTOR<sslOBJECT *, 8>::PushBack(sslOBJECT *const &val)
{
    if (this->allocated >= this->nElem + 1) {
        // spare capacity: store straight into the tail slot
        this->pData[this->nElem] = val;
    } else {
        // must grow: save `val` first in case it aliases storage about to be reallocated
        sslOBJECT *saved = val;
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
