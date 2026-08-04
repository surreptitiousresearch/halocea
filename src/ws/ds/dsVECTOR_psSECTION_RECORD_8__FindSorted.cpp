#include "dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ps/psSECTION_RECORD.h"

// dsVECTOR<psSECTION_RECORD,8>::FindSorted<ORS_CMP,dsSTRID> @ 0x82519238
// Binary search of the sort-order index for the record whose interned key equals `key`.
// This vector holds the sort permutation: element[i].sortedIdx names the record (in cmp.arr) that
// occupies sort position i. Records are ordered by the pointer value of their interned key id
// (dsSTRID::id), which the pooled/hashed strings guarantee is a consistent total order.
// Returns the sort-position index of the match, or -1 if not present.
template<>
template<>
int dsVECTOR<psSECTION_RECORD, 8>::FindSorted<ORS_CMP, dsSTRID>(
    const dsSTRID &key, const ORS_CMP &cmp) const
{
    int hi = this->nElem;
    int lo = 0;
    if (hi != 0) {
        do {
            const char *keyId = key.id;
            int mid = (hi + lo) >> 1;
            const char *elemId = (*cmp.arr)[this->pData[mid].sortedIdx].keyStrId.id;
            if (elemId < keyId) {
                lo = mid + 1;
            } else {
                if (keyId >= elemId) // neither less nor greater: exact match
                    return mid;
                hi = mid;
            }
        } while (lo != hi);
    }
    return -1;
}
