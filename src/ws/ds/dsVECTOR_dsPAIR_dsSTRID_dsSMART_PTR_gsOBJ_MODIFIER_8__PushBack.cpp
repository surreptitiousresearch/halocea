#include <new>
#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSMART_PTR.h"
#include "dsREF_COUNT.h"

struct gsOBJ_MODIFIER_BASE_DESC; // boundary — gs subsystem object-modifier descriptor (pointee only)

// dsVECTOR<dsPAIR<dsSTRID,dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,...>>,8>::PushBack @ 0x826D2678
// Append a copy of `val` (its dsPAIR copy ctor bumps the smart pointer's intrusive refcount) and
// return a reference to the new last element. When the vector must grow, `val` is first copy-
// constructed into a stack temporary before Realloc (which may move/free the backing store); the
// new slot is then copy-constructed from that temporary, whose own reference is released at the end.
template<>
dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > > &
dsVECTOR<dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                     _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > >, 8>::PushBack(
        const dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                          _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > > &val)
{
    typedef dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                        _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > > PairT;

    if (this->allocated >= this->nElem + 1) {
        PairT *slot = &this->pData[this->nElem];
        if (slot)
            new (slot) PairT(val);
    } else {
        PairT temp(val);

        int newCapacity = this->nElem + 1;
        if (2 * this->allocated > newCapacity)
            newCapacity = 2 * this->allocated;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);

        PairT *slot = &this->pData[this->nElem];
        if (slot)
            new (slot) PairT(temp);
        temp.val.DeletePointee();
    }
    ++this->nElem;
    return this->Back();
}
