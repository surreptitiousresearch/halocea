#include "dsVECTOR.h"

struct dsCMP; // boundary — default ascending integer comparator (stateless policy)

// dsVECTOR<int,8>::InsertSorted<dsCMP> @ 0x823B7B40
// Binary-search insert into an ascending-sorted int vector. On a miss, splice `val` at the
// lower-bound index and return it. On a match at `mid`, `dup` selects the duplicate policy.
template<>
template<>
int dsVECTOR<int, 8>::InsertSorted<dsCMP>(const int &val, const dsCMP &, INS_SORT dup)
{
    int hi = this->nElem;
    int lo = 0;
    if (!hi) {
        this->Insert(lo, &val, 1);
        return lo;
    }

    int key = val;
    int mid;
    int *slot;
    while (true) {
        mid = (hi + lo) >> 1;
        slot = &this->pData[mid];
        if (*slot < key) {
            lo = mid + 1;
        } else if (key >= *slot) { // *slot >= key and key >= *slot => equal match
            break;
        } else {
            hi = mid;
        }
        if (lo == hi) {
            this->Insert(lo, &val, 1);
            return lo;
        }
    }

    // duplicate key already present at `mid`
    if (dup == INS_DUP_REPLACE) {
        *slot = key;
    } else if (dup == INS_DUP_IGNORE) {
        // keep the existing element
    } else if (dup == INS_DUP_RIGHT) {
        this->Insert((hi + lo) >> 1, &val, 1);
    } else {
        return -1;
    }
    return mid;
}
