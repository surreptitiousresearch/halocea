#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"

// dsVECTOR<tplLOD_DEF,8>::PushBack @ 0x82B0DEE0
// Append `val` to the tail (a trivially relocatable POD record), growing the backing store first
// when there is no spare capacity, and return a reference to the newly stored element.
template<>
tplLOD_DEF &dsVECTOR<tplLOD_DEF, 8>::PushBack(const tplLOD_DEF &val)
{
    if (this->allocated >= this->nElem + 1) {
        this->pData[this->nElem] = val;
    } else {
        tplLOD_DEF saved = val; // save before Realloc may invalidate `val`
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
