#include <new>
#include "dsVECTOR.h"
#include "../../headers/hcex/HCEX_MODEL_REGION.h"

// dsVECTOR<HCEX_MODEL_REGION,8>::PushBack @ 0x823E50B8
// Append a copy of `val` (a non-trivial element: a dsTSTRING name plus two dsVECTOR sub-lists), via
// the copy-constructor. When there is spare capacity the copy is made directly into the next slot;
// otherwise `val` is first copy-constructed into a stack temporary (val may alias storage that
// Realloc is about to invalidate), the backing store is grown, and the temporary is copy-constructed
// again into the new tail slot. DEVIATION: the decompiler shows the temporary's destructor inlined
// explicitly (release name's buffer, destroy objNames, free objIds) right before the return; that is
// just its ordinary scope-exit destructor call, reproduced here as a real C++ local instead of by
// hand. Returns the new last slot.
template<>
HCEX_MODEL_REGION &dsVECTOR<HCEX_MODEL_REGION, 8>::PushBack(const HCEX_MODEL_REGION &val)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        HCEX_MODEL_REGION *slot = &this->pData[nElem];
        if (slot)
            new (slot) HCEX_MODEL_REGION(val);
    } else {
        HCEX_MODEL_REGION saved(val); // copy before Realloc may invalidate `val`

        int allocated = this->allocated;
        int newCapacity = this->nElem + 1;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        HCEX_MODEL_REGION *slot = &this->pData[this->nElem];
        if (slot)
            new (slot) HCEX_MODEL_REGION(saved);
        // `saved` destructs here at scope exit (matches the decompiler's inlined tail).
    }
    ++this->nElem;
    return this->Back();
}
