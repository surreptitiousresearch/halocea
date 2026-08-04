#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless three-way comparator (compare inlined below)

// dsVECTOR<dsPAIR<int,dsTSTRING<char>>,8>::FindSorted<dsCMP,int> @ 0x825CAA98
// Binary search of an ascending-by-key vector for the pair whose int key equals `key`;
// returns its index or -1.
template<>
template<>
int dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>::FindSorted<dsCMP, int>(
        const int &key, const dsCMP &cmp) const
{
    (void)cmp;
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        const dsPAIR<int, dsTSTRING<char> > *elems = this->pData;
        int searchKey = key;
        do {
            int mid = (hi + lo) >> 1;
            int midKey = elems[mid].key;
            if (midKey < searchKey)
                lo = mid + 1;
            else if (midKey == searchKey) // inlined dsCMP three-way equal test
                return mid;
            else
                hi = mid;
        } while (lo != hi);
    }
    return -1;
}
