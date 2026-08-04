#include "dsVECTOR.h"
#include "WEAK_PTR.h"

#include "entENTITY.h"

// boundary — global placement copy-construct helper: New<T>(slot, src) === new (slot) T(*src).
#include <new>
template<class T> static T *New(T *slot, const T *src) { return ::new (static_cast<void *>(slot)) T(*src); }
// boundary — os interlocked increment of a 32-bit word; returns the new value.
extern "C" int osLockedIncrement(volatile int *dest);

// dsVECTOR<ds::WEAK_PTR<entENTITY>,8>::PushBack @ 0x823E2348
// Append a copy of the weak reference `val` at the tail, growing the backing store first when
// there is no spare capacity, and return a reference to the newly stored element. Copying a
// WEAK_PTR shares its handle and bumps the handle refcount.
template<>
ds::WEAK_PTR<entENTITY> &dsVECTOR<ds::WEAK_PTR<entENTITY>, 8>::PushBack(
        const ds::WEAK_PTR<entENTITY> &val)
{
    if (this->allocated >= this->nElem + 1) {
        New<ds::WEAK_PTR<entENTITY>>(&this->pData[this->nElem], &val);
    } else {
        // Copy-construct the weak ref by value first (it may alias storage about to be
        // reallocated): share the handle and take a reference on it.
        ds::WEAK_PTR<entENTITY> saved;
        saved.pHandle = val.pHandle;
        if (saved.pHandle)
            osLockedIncrement(&saved.pHandle->refCount);
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        New<ds::WEAK_PTR<entENTITY>>(&this->pData[this->nElem], &saved);
        saved.~WEAK_PTR(); // release the temporary's reference (New copied it into the slot)
    }
    ++this->nElem;
    return this->Back();
}
