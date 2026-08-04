#include <new>
#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSHARED_PTR.h"
#include "Deleter.h"
#include "../anim/animLAYER_BASE.h"

// dsVECTOR<dsPAIR<dsSTRID,dsSHARED_PTR<animLAYER_BASE,0,Deleter<animLAYER_BASE>>>,8>::PushBack
// @ 0x8267EB38
// Append a copy of `val`: the dsSTRID key is copied directly, and the dsSHARED_PTR value is copy-
// constructed in place (bumping the shared count). When the vector must grow, the value is first
// copy-constructed into a stack temporary (the key snapshotted separately) before Realloc; the new
// slot is then copy-constructed from that temporary, whose own reference is released at the end.
template<>
dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > > &
dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > >, 8>::PushBack(
        const dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > > &val)
{
    typedef dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > SharedPtrT;
    typedef dsPAIR<dsSTRID, SharedPtrT> PairT;

    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        PairT *slot = &this->pData[nElem];
        if (slot) {
            slot->key.id = val.key.id;
            new (&slot->val) SharedPtrT(val.val);
        }
    } else {
        const char *tempId = val.key.id;
        SharedPtrT tempValue(val.val);

        int allocated = this->allocated;
        int newCapacity = this->nElem + 1;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        PairT *slot = &this->pData[this->nElem];
        if (slot) {
            slot->key.id = tempId;
            new (&slot->val) SharedPtrT(tempValue);
        }
        tempValue.Reset();
    }
    ++this->nElem;
    return this->Back();
}
