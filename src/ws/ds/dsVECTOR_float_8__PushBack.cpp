#include "dsVECTOR.h"

// dsVECTOR<float,8>::PushBack @ 0x823E20C8
// Append `val` to the tail, growing the backing store first when there is no spare capacity,
// and return a reference to the newly stored element.
// The saved copy rides an FP register (float-slot ABI) in the disassembly; it is a plain float.
template<>
float &dsVECTOR<float, 8>::PushBack(const float &val)
{
    if (this->allocated >= this->nElem + 1) {
        this->pData[this->nElem] = val;
    } else {
        float saved = val; // save first: `val` may alias storage about to be reallocated
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        this->pData[this->nElem] = saved;
    }
    ++this->nElem;
    return this->Back();
}
