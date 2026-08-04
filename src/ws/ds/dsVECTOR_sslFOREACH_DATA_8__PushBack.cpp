#include "dsVECTOR.h"
#include "../ssl/sslFOREACH_DATA.h"

// boundary — global placement copy-construct helper: New<T>(slot, src) === new (slot) T(*src).
#include <new>
template<class T> static T *New(T *slot, const T *src) { return ::new (static_cast<void *>(slot)) T(*src); }

// dsVECTOR<sslFOREACH_DATA,8>::PushBack @ 0x82AF40C8
// Append a copy of `val` to the tail, growing the backing store first when there is no spare
// capacity, and return a reference to the newly stored element. Each element carries a
// ref-counted sslCLASS_REF, so the value is copy-CONSTRUCTED into the raw tail slot (not assigned).
template<>
sslFOREACH_DATA &dsVECTOR<sslFOREACH_DATA, 8>::PushBack(const sslFOREACH_DATA &val)
{
    if (this->allocated >= this->nElem + 1) {
        New<sslFOREACH_DATA>(&this->pData[this->nElem], &val);
    } else {
        sslFOREACH_DATA saved = val; // save first: `val` may alias storage about to be reallocated
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        New<sslFOREACH_DATA>(&this->pData[this->nElem], &saved);
    }
    ++this->nElem;
    return this->Back();
}
