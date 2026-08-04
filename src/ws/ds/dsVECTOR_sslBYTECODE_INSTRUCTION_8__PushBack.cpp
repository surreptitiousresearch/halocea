#include "dsVECTOR.h"
#include "../ssl/sslBYTECODE_INSTRUCTION.h"

// dsVECTOR<sslBYTECODE_INSTRUCTION,8>::PushBack @ 0x83089E10
// Append `val` to the tail (a trivially relocatable POD instruction), growing the backing store
// first when there is no spare capacity, and return a reference to the newly stored element.
template<>
sslBYTECODE_INSTRUCTION &dsVECTOR<sslBYTECODE_INSTRUCTION, 8>::PushBack(const sslBYTECODE_INSTRUCTION &val)
{
    if (this->allocated >= this->nElem + 1) {
        this->pData[this->nElem] = val;
    } else {
        sslBYTECODE_INSTRUCTION saved = val; // save before Realloc may invalidate `val`
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
