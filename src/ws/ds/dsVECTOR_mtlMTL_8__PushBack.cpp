#include <new>
#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"

// dsVECTOR<mtlMTL,8>::PushBack @ 0x82C2CEF8
// Append a copy of `value` to the end of the vector, growing storage (doubling policy) if needed,
// and return a reference to the newly appended slot (via Back()).
//
// DEVIATION NOTE: when growth is required, the compiler places a defensive copy of `value` in raw
// stack storage BEFORE calling Realloc — `value` may alias a live element that Realloc's underlying
// dlRealloc could move/free, so the source has to be preserved across the reallocation. Modeled here
// as placement-new into a raw byte buffer + explicit destructor call (not a normal C++ automatic
// local), matching the disassembly's construct/destroy pair exactly (mtlMTL::mtlMTL(&tmp,val) /
// mtlMTL::~mtlMTL(&tmp)) rather than relying on scope-exit destruction.
template<>
mtlMTL &dsVECTOR<mtlMTL, 8>::PushBack(const mtlMTL &value)
{
    int liveCount = this->nElem;
    if (this->allocated >= liveCount + 1) {
        mtlMTL *newSlot = &this->pData[liveCount];
        if (newSlot)
            new (newSlot) mtlMTL(value); // copy-construct the appended element
    } else {
        alignas(mtlMTL) unsigned char deferredCopyStorage[sizeof(mtlMTL)];
        mtlMTL *deferredCopy = reinterpret_cast<mtlMTL *>(deferredCopyStorage);
        new (deferredCopy) mtlMTL(value); // stack copy, survives the reallocation below

        int capacity = this->allocated;
        int needed = this->nElem + 1;
        if (2 * capacity > needed)
            needed = 2 * capacity;
        if (needed > capacity)
            this->Realloc(needed);

        mtlMTL *newSlot = &this->pData[this->nElem];
        if (newSlot)
            new (newSlot) mtlMTL(*deferredCopy);
        deferredCopy->~mtlMTL();
    }
    ++this->nElem;
    return this->Back();
}
