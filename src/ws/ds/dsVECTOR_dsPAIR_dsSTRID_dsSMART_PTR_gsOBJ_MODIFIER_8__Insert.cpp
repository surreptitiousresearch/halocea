#include <new>
#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSMART_PTR.h"
#include "dsREF_COUNT.h"
#include "ds_assert_boundary.h"

struct gsOBJ_MODIFIER_BASE_DESC; // boundary — gs subsystem object-modifier descriptor (pointee only)

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsVECTOR<dsPAIR<dsSTRID,dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,...>>,8>::Insert (range overload)
// @ 0x826D1238
// Splice `num` pairs from the source range starting at `begin` in at index `pos`. If growing
// requires a Realloc and `begin` pointed inside the OLD backing store, `begin` is rebased onto the
// NEW store first (the decompiled self-aliasing fixup) before shifting the tail and placement-copy-
// constructing each new pair (dsPAIR's copy ctor bumps the smart pointer's intrusive refcount).
template<>
void dsVECTOR<dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                          _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > >, 8>::Insert(
        int pos,
        const dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                          _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > > *begin,
        int num)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || num < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && num >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            382,
            "pos", pos, "nElem", this->nElem, "num", num);
    if (num == 0)
        return;

    typedef dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                        _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > > PairT;

    PairT *oldData = this->pData;
    if (this->allocated < this->nElem + num) {
        int newCapacity = this->nElem + num;
        if (2 * this->allocated > newCapacity)
            newCapacity = 2 * this->allocated;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
    }

    // Rebase `begin` if it pointed inside the backing store and Realloc relocated it.
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + num], &this->pData[pos], 8 * (this->nElem - pos));
    for (int destIdx = pos; destIdx < pos + num; ++destIdx, ++begin) {
        PairT *slot = &this->pData[destIdx];
        if (slot)
            new (slot) PairT(*begin); // dsPAIR copy ctor: bumps the smart pointer's refcount
    }
    this->nElem += num;
}
