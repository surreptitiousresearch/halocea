#include "dsAFFIX_BUILDER.h"
#include <new>

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::PushBack @ 0x82A77AD0
// Append a copy of `value` and return a reference to the new last element. dsAFFIX_BUILDER::AFFIX
// has a non-trivial copy constructor/destructor (its name string shares a refcounted buffer and
// its params vector is deep-copied), so the affix is copy-constructed into place. When the vector
// must grow, `value` is first buffered into a temporary (it may alias the backing store), then
// copy-constructed into the grown slot after Realloc.
template<>
dsAFFIX_BUILDER::AFFIX &dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::PushBack(
        const dsAFFIX_BUILDER::AFFIX &value)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        // Spare capacity: copy-construct directly into the next slot.
        new (&this->pData[nElem]) dsAFFIX_BUILDER::AFFIX(value);
    } else {
        // Full: buffer the affix (it may alias pData) before reallocating.
        dsAFFIX_BUILDER::AFFIX temp(value);
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        new (&this->pData[this->nElem]) dsAFFIX_BUILDER::AFFIX(temp);
        // temp destroyed here (the compiler inlined ~AFFIX: destroy the params vector and release
        // the name buffer).
    }
    ++this->nElem;
    return this->Back();
}
