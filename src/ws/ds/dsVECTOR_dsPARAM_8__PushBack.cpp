#include "dsVECTOR.h"
#include "dsPARAM.h"

// boundary — global placement copy-construct helper: New<T>(slot, src) === new (slot) T(*src).
#include <new>
template<class T> static T *New(T *slot, const T *src) { return ::new (static_cast<void *>(slot)) T(*src); }

// dsVECTOR<dsPARAM,8>::PushBack @ 0x823E2180
// Append a copy of `val`. A dsPARAM is {dsSTRID id, dsDATA data}; the id is a plain interned pointer
// and the data is copied through dsDATA::SetValue. With spare capacity the copy is constructed
// directly into the next slot. Otherwise the value is staged into a temporary (val may alias the
// storage about to be reallocated), the backing store is grown, the staged value is moved into the
// new tail slot, and the staged dsDATA value is destroyed. Returns the new last slot.
template<>
dsPARAM &dsVECTOR<dsPARAM, 8>::PushBack(const dsPARAM &val)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        New<dsPARAM>(&this->pData[nElem], &val);
    } else {
        dsPARAM staged;
        staged.id.id = val.id.id;
        staged.data.type = nullptr;
        staged.data.SetValue(&val.data);

        int allocated = this->allocated;
        int newCapacity = 2 * allocated;
        if (2 * allocated <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        New<dsPARAM>(&this->pData[this->nElem], &staged);
        if (staged.data.type)
            staged.data.type->Destroy(&staged.data.storage);
    }
    ++this->nElem;
    return this->Back();
}
