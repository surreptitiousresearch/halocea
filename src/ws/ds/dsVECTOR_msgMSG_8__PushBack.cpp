#include "dsVECTOR.h"
#include "../msg/msgMSG.h"

// dsVECTOR<msgMSG,8>::PushBack @ 0x82AC54C0
// Append a copy of the 24-byte message envelope and return a reference to the new last element.
// msgMSG is trivially copyable (the original inlines a 6-word copy), so the record is assigned into
// place. When the vector must grow, `val` is first buffered into a temporary (it may alias the
// backing store), then assigned into the grown slot after Realloc.
template<>
msgMSG &dsVECTOR<msgMSG, 8>::PushBack(const msgMSG &val)
{
    if (this->allocated >= this->nElem + 1) {
        // Spare capacity: copy directly into the next slot.
        msgMSG *slot = &this->pData[this->nElem];
        if (slot)
            *slot = val;
    } else {
        // Full: buffer the envelope (it may alias pData) before reallocating.
        msgMSG temp = val;
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        msgMSG *slot = &this->pData[this->nElem];
        if (slot)
            *slot = temp;
    }
    ++this->nElem;
    return this->Back();
}
