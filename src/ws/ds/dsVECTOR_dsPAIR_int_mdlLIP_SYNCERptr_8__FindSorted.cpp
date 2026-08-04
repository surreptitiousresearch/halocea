#include "dsVECTOR.h"
#include "dsPAIR.h"

struct mdlLIP_SYNCER; // boundary — ws-engine animation lip-syncer (pair value is a pointer)
struct dsCMP {};      // boundary — key comparator (empty; comparison inlined)

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::FindSorted<dsCMP,int> @ 0x823D4198
// Binary search of the ascending-key vector for `key`; returns its index or -1.
template<>
template<>
int dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::FindSorted<dsCMP, int>(
        const int &key, const dsCMP &cmp) const
{
    (void)cmp;
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        const dsPAIR<int, mdlLIP_SYNCER *> *elems = this->pData;
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
