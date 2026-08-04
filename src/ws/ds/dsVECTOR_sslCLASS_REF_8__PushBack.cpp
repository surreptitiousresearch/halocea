#include "dsVECTOR.h"

#include "sslCLASS_REF.h"

// boundary — global placement copy-construct helper: New<T>(slot, src) === new (slot) T(*src).
#include <new>
template<class T> static T *New(T *slot, const T *src) { return ::new (static_cast<void *>(slot)) T(*src); }

// dsVECTOR<sslCLASS_REF,8>::PushBack @ 0x825D0D38
// Append a copy of `val` at the tail, growing the backing store first when there is no spare
// capacity, and return a reference to the newly stored element. Each element carries a
// ref-counted sslCLASS_REF, so the value is copy-CONSTRUCTED into the raw tail slot (not assigned).
template<>
sslCLASS_REF &dsVECTOR<sslCLASS_REF, 8>::PushBack(const sslCLASS_REF &val)
{
    if (this->allocated >= this->nElem + 1) {
        New<sslCLASS_REF>(&this->pData[this->nElem], &val);
    } else {
        sslCLASS_REF saved = val; // save first: `val` may alias storage about to be reallocated
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        New<sslCLASS_REF>(&this->pData[this->nElem], &saved);
    }
    ++this->nElem;
    return this->Back();
}
