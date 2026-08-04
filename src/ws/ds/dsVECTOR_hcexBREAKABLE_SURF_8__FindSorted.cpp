#include "dsVECTOR.h"
#include "../../headers/hcex/hcex_breakable_surf.h"

struct dsCMP {}; // boundary — stateless three-way comparator; the (bsp, idx) compare is inlined

// dsVECTOR<hcexBREAKABLE_SURF,8>::FindSorted<dsCMP,hcexBREAKABLE_SURF> @ 0x823B0D68
// Binary search of a vector kept ascending by the (bsp, idx) lexicographic key for the record that
// matches `key`; returns its index, or -1 on a miss. The dsCMP three-way ordering (signed compare
// of bsp, then idx) is inlined here.
template<>
template<>
int dsVECTOR<hcexBREAKABLE_SURF, 8>::FindSorted<dsCMP, hcexBREAKABLE_SURF>(
        const hcexBREAKABLE_SURF &key, const dsCMP &) const
{
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        const hcexBREAKABLE_SURF *pData = this->pData;
        int keyBsp = key.bsp;
        int keyIdx = key.idx;
        do {
            int mid = (int)((unsigned)(hi + lo) >> 1);
            const hcexBREAKABLE_SURF &elem = pData[mid];
            // dsCMP three-way ordering by (bsp, idx), signed.
            bool elemLessThanKey = (elem.bsp == keyBsp) ? (elem.idx < keyIdx)
                                                        : (elem.bsp < keyBsp);
            if (elemLessThanKey) {
                lo = mid + 1;
            } else {
                bool keyLessThanElem = (elem.bsp == keyBsp) ? (keyIdx < elem.idx)
                                                            : (keyBsp < elem.bsp);
                if (!keyLessThanElem)
                    return mid; // equal
                hi = mid;
            }
        } while (lo != hi);
    }
    return -1;
}
