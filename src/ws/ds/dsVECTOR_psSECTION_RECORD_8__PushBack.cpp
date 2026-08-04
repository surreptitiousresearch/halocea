#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"
#include <new>

// dsVECTOR<psSECTION_RECORD,8>::PushBack @ 0x82519C68
// Append a copy of `value` and return a reference to the new last element. psSECTION_RECORD has a
// non-trivial copy constructor/destructor (its dsDATA `value` member deep-copies / releases its
// held value), so the record is copy-constructed into place. When the vector must grow, `value` is
// first buffered into a temporary (it may alias the backing store), then copy-constructed into the
// grown slot after Realloc.
template<>
psSECTION_RECORD &dsVECTOR<psSECTION_RECORD, 8>::PushBack(const psSECTION_RECORD &value)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        // Spare capacity: copy-construct directly into the next slot.
        new (&this->pData[nElem]) psSECTION_RECORD(value);
    } else {
        // Full: buffer the record (it may alias pData) before reallocating.
        psSECTION_RECORD temp(value);
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        new (&this->pData[this->nElem]) psSECTION_RECORD(temp);
        // temp destroyed here (~psSECTION_RECORD inlined: the dsDATA `value` releases its held
        // value via value.type->Destroy).
    }
    ++this->nElem;
    return this->Back();
}
