#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::PushBack @ 0x8264C368
// Append `val` to the tail (a dsSTRID is just an interned const char*), growing the backing store
// first when there is no spare capacity, and return a reference to the newly stored element.
template<>
dsSTRID &dsVECTOR<dsSTRID, 8>::PushBack(const dsSTRID &val)
{
    if (this->allocated >= this->nElem + 1) {
        // spare capacity: store straight into the tail slot
        this->pData[this->nElem].id = val.id;
    } else {
        // must grow: save the id first in case `val` aliases storage about to be reallocated
        const char *savedId = val.id;
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        this->pData[this->nElem].id = savedId;
    }
    ++this->nElem;
    return this->Back();
}
