#include "dsVECTOR.h"
#include "../ssl/sslCLASS_ELEMENT.h"

// dsVECTOR<sslCLASS_ELEMENT,8>::PushBack @ 0x82A3B748
// Append `val` to the tail (a trivially relocatable POD record), growing the backing store first
// when there is no spare capacity, and return a reference to the newly stored element.
template<>
sslCLASS_ELEMENT &dsVECTOR<sslCLASS_ELEMENT, 8>::PushBack(const sslCLASS_ELEMENT &val)
{
    if (this->allocated >= this->nElem + 1) {
        this->pData[this->nElem] = val;
    } else {
        sslCLASS_ELEMENT saved = val; // save before Realloc may invalidate `val`
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
