#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"

// dsVECTOR<rendSR_LAMP_DATA,8>::PushBack @ 0x829ECC58
// Append `val` to the tail (a trivially relocatable packed word), growing the backing store first
// when there is no spare capacity, and return a reference to the newly stored element.
template<>
rendSR_LAMP_DATA &dsVECTOR<rendSR_LAMP_DATA, 8>::PushBack(const rendSR_LAMP_DATA &val)
{
    if (this->allocated >= this->nElem + 1) {
        this->pData[this->nElem] = val;
    } else {
        unsigned int savedData = val.data; // save before Realloc may invalidate `val`
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        this->pData[this->nElem].data = savedData;
    }
    ++this->nElem;
    return this->Back();
}
