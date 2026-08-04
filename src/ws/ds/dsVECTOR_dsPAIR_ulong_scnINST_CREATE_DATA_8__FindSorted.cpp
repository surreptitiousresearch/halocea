#include "dsVECTOR.h"
#include "dsPAIR.h"

struct scnINST_CREATE_DATA { char _opaque[88]; }; // boundary — scn instance spawn record; only the pair key is read
struct dsCMP {};                                  // boundary — key comparator (empty; comparison inlined)

// dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>::FindSorted<dsCMP,unsigned long> @ 0x823D4090
// Binary search of the ascending-key vector for `key`; returns its index or -1.
template<>
template<>
int dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::FindSorted<dsCMP, unsigned long>(
        const unsigned long &key, const dsCMP &cmp) const
{
    (void)cmp;
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        const dsPAIR<unsigned long, scnINST_CREATE_DATA> *elems = this->pData;
        const unsigned int target = key;
        do {
            int mid = (hi + lo) >> 1;
            unsigned int midKey = elems[mid].key;
            if (midKey < target)
                lo = mid + 1;
            else if (target >= midKey) // equal
                return mid;
            else
                hi = mid; // decompiler recomputes (hi+lo)>>1 == mid
        } while (lo != hi);
    }
    return -1;
}
