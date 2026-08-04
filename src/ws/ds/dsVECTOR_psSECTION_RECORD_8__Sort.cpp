#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"

// dsVECTOR<psSECTION_RECORD,8>::Sort<ORS_CMP,ORS_SWAP> @ 0x8251A0A0
// In-place sort of the record vector. ORS_CMP orders two records by the interned key id
// (dsSTRID::id pointer) of the record that each one's `sortedIdx` points at within cmp.arr; ORS_SWAP
// exchanges only the `sortedIdx` permutation field between two records (not the whole record).
// For up to 100 records this uses an O(n^2) selection sort (bubbling each smaller key toward the
// front); larger vectors delegate to HeapSort. `nElem` is reloaded every inner iteration, matching
// the disasm (the comparator/swap could in principle observe a resized vector).
template<>
template<>
void dsVECTOR<psSECTION_RECORD, 8>::Sort<ORS_CMP, ORS_SWAP>(
        const ORS_CMP &cmp, const ORS_SWAP &swap)
{
    (void)swap; // stateless: the swap operation is inlined below
    int nElem = this->nElem;
    if (nElem <= 100) {
        for (int i = 0; i < nElem - 1; ++i) {
            for (int j = i + 1; j < nElem; ++j) {
                // ORS_CMP: order by the key id of the record each sortedIdx points at.
                const char *keyI = (*cmp.arr)[this->pData[i].sortedIdx].keyStrId.id;
                const char *keyJ = (*cmp.arr)[this->pData[j].sortedIdx].keyStrId.id;
                if (keyJ < keyI) {
                    // ORS_SWAP: exchange the two records' sortedIdx fields only.
                    int tmp = this->pData[i].sortedIdx;
                    this->pData[i].sortedIdx = this->pData[j].sortedIdx;
                    this->pData[j].sortedIdx = tmp;
                }
                nElem = this->nElem;
            }
        }
    } else {
        this->HeapSort(cmp, swap);
    }
}
