#include "dsVECTOR.h"

// dsVECTOR<int,8>::PushBack @ 0x82522818
// Append `val` to the tail, growing the backing store first when there is no spare capacity,
// and return a reference to the newly stored element.
template<>
int &dsVECTOR<int, 8>::PushBack(const int &val)
{
    if (this->allocated >= this->nElem + 1) {
        this->pData[this->nElem] = val;
    } else {
        int saved = val; // save first: `val` may alias storage about to be reallocated
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
