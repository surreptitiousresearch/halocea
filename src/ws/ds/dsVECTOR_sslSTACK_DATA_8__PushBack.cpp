#include "dsVECTOR.h"

#include "sslSTACK_DATA.h"

// boundary — global placement copy-construct helper: New<T>(slot, src) === new (slot) T(*src).
#include <new>
template<class T> static T *New(T *slot, const T *src) { return ::new (static_cast<void *>(slot)) T(*src); }

// dsVECTOR<sslSTACK_DATA,8>::PushBack @ 0x82AF3FA8
// Append a copy of `val` at the tail, growing the backing store first when there is no spare
// capacity, and return a reference to the newly stored element. When a reallocation is needed the
// value is first copied into a scratch slot (the compiler inlines sslSTACK_DATA's default ctor,
// which zero-inits it to TYPE_INT, then operator=), so `val` survives the store even if it aliased
// the storage being reallocated; the scratch slot is then copy-constructed into the tail and its
// inlined dtor (sslSTACK_DATA::Cleanup) runs at scope exit.
template<>
sslSTACK_DATA &dsVECTOR<sslSTACK_DATA, 8>::PushBack(const sslSTACK_DATA &val)
{
    if (this->allocated >= this->nElem + 1) {
        New<sslSTACK_DATA>(&this->pData[this->nElem], &val);
    } else {
        sslSTACK_DATA saved; // default-constructs to (TYPE_INT, 0) per the inlined ctor
        saved = val;         // sslSTACK_DATA::operator=
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        New<sslSTACK_DATA>(&this->pData[this->nElem], &saved);
    }
    ++this->nElem;
    return this->Back();
}
