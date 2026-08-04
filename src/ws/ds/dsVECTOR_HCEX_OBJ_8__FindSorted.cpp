#include "dsVECTOR.h"
#include "../../hcex/HCEX_OBJ.h" // reuse the real HCEX_OBJ layout (id@0)

struct HCEX_OBJ_CMP {}; // boundary — comparator over HCEX_OBJ ids (compare inlined)

// dsVECTOR<HCEX_OBJ,8>::FindSorted<HCEX_OBJ_CMP,int> @ 0x823ED770
// Binary search of the ascending-id object vector for `key`; returns its index or -1.
template<>
template<>
int dsVECTOR<HCEX_OBJ, 8>::FindSorted<HCEX_OBJ_CMP, int>(
        const int &key, const HCEX_OBJ_CMP &cmp) const
{
    (void)cmp;
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        const int target = key;
        const HCEX_OBJ *elems = this->pData;
        do {
            int mid = (hi + lo) >> 1;
            int midId = elems[mid].id;
            if (midId < target)
                lo = mid + 1;
            else if (midId == target) // decompiler emits the signed-compare equality idiom
                return mid;
            else
                hi = mid;
        } while (lo != hi);
    }
    return -1;
}
