#include "dsVECTOR.h"
#include "../../headers/hcex/HCEX_EFFECT.h"
#include <new>

// dsVECTOR<HCEX_EFFECT,8>::PushBack @ 0x823E51A0
// Append a copy of `value` and return a reference to the new last element. HCEX_EFFECT has a
// non-trivial copy constructor/destructor, so the value is copy-constructed into place. When the
// vector must grow, `value` is first buffered into a temporary (it may alias the backing store),
// then copy-constructed into the grown slot after Realloc.
template<>
HCEX_EFFECT &dsVECTOR<HCEX_EFFECT, 8>::PushBack(const HCEX_EFFECT &value)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        // Spare capacity: copy-construct directly into the next slot.
        new (&this->pData[nElem]) HCEX_EFFECT(value);
    } else {
        // Full: buffer the value (it may alias pData) before reallocating.
        HCEX_EFFECT temp(value);
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        new (&this->pData[this->nElem]) HCEX_EFFECT(temp);
        // temp destroyed here (the compiler inlined ~HCEX_EFFECT: release the name buffer and
        // destroy the locations vector).
    }
    ++this->nElem;
    return this->Back();
}
