#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

// dsVECTOR<m3dMATR,8>::PushBack @ 0x823E2418
// Append `val` to the tail of the vector, growing the backing store first if there is no spare
// capacity, and return a reference to the newly stored element.
template<>
m3dMATR &dsVECTOR<m3dMATR, 8>::PushBack(const m3dMATR &val)
{
    if (this->allocated >= this->nElem + 1) {
        // spare capacity available: store straight into the tail slot (64-byte copy)
        this->pData[this->nElem] = val;
    } else {
        // must grow: save `val` first in case it aliases storage about to be reallocated
        m3dMATR saved = val;
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
