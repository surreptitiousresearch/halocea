#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "../../hcex/hcex_light.h" // reuse the real HCEX_LIGHT layout

struct dsCMP {}; // boundary — key comparator (empty; comparison inlined)

// dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8>::FindSorted<dsCMP,int> @ 0x823D4208
// Binary search of the ascending-key vector for `key`; returns its index or -1.
template<>
template<>
int dsVECTOR<dsPAIR<int, HCEX_LIGHT>, 8>::FindSorted<dsCMP, int>(
        const int &key, const dsCMP &cmp) const
{
    (void)cmp;
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        const dsPAIR<int, HCEX_LIGHT> *elems = this->pData;
        const int target = key;
        do {
            int mid = (hi + lo) >> 1;
            int midKey = elems[mid].key;
            if (midKey < target)
                lo = mid + 1;
            else if (midKey == target) // decompiler emits the signed-compare equality idiom
                return mid;
            else
                hi = mid;
        } while (lo != hi);
    }
    return -1;
}
