#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSMART_PTR.h"
#include "dsREF_COUNT.h"

struct gsOBJ_MODIFIER_BASE_DESC; // boundary — gs subsystem object-modifier descriptor (pointee only)
struct dsCMP {};                 // boundary — dsSTRID ordering policy (compares interned id pointers)

// dsVECTOR<dsPAIR<dsSTRID,dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,...>>,8>::Find<dsCMP,dsSTRID>
// @ 0x826CC390
// Linear scan from `startPos` for the first pair whose dsSTRID key is equivalent to `key` under the
// dsCMP strict-weak ordering (which orders by the interned id pointer). Equivalence is the usual
// !(elem<key) && !(key<elem). Returns the element index, or -1 if none / startPos out of range.
template<>
template<>
int dsVECTOR<dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                         _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > >, 8>::
    Find<dsCMP, dsSTRID>(const dsSTRID &key, const dsCMP &cmp, int startPos) const
{
    (void)cmp; // comparator is empty; the id-pointer ordering is emitted inline
    if (startPos >= this->nElem)
        return -1;

    for (int scanIndex = startPos; scanIndex != this->nElem; ++scanIndex) {
        const dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                          _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > > *element =
            &this->pData[scanIndex];
        bool keysDiffer;
        if (element->key.id < key.id)          // element key orders before the search key
            keysDiffer = true;
        else
            keysDiffer = (key.id < element->key.id); // search key orders before the element key
        if (!keysDiffer)                       // equivalent
            return scanIndex;
    }
    return -1;
}
