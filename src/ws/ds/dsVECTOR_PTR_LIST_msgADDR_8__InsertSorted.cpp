#include "dsVECTOR.h"
#include "PTR_LIST.h"

struct msgADDR; // boundary — msg subsystem message address

// boundary — msg comparator: orders a message-address bucket (PTR_LIST) against another bucket.
// Returns <0 / 0 / >0 like strcmp (element-vs-key).
struct msgCMP_ADDR2 {
    int cmp(const ds::PTR_LIST<msgADDR> *elem, const ds::PTR_LIST<msgADDR> *key) const; // boundary
};

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::InsertSorted<msgCMP_ADDR2> @ 0x8255B9D8
// Binary-search insert of the bucket `val` into the ascending-sorted vector, ordered by `cmp`. On a
// miss, splice `val` at the lower-bound index and return it. On a match at `mid`, `dup` selects the
// duplicate policy (replace overwrites the matched bucket's list fields in place).
template<>
template<>
int dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::InsertSorted<msgCMP_ADDR2>(
        const ds::PTR_LIST<msgADDR> &val, const msgCMP_ADDR2 &cmp, INS_SORT dup)
{
    int lo = 0;
    int hi = this->nElem;
    int mid;
    int order;
    while (true) {
        // descend while the probed element sorts after `val`
        while (true) {
            if (lo == hi) {
                this->Insert(lo, &val, 1);
                return lo;
            }
            mid = (lo + hi) >> 1;
            order = cmp.cmp(&this->pData[mid], &val);
            if (order <= 0)
                break;
            hi = mid; // element > key: search the lower half
        }
        if (order >= 0) // order == 0: exact match at mid
            break;
        lo = mid + 1;   // element < key: search the upper half
    }

    // duplicate key already present at `mid`
    if (dup == INS_DUP_REPLACE) {
        ds::PTR_LIST<msgADDR> *slot = &this->pData[mid];
        slot->pHead = val.pHead;
        slot->pTail = val.pTail;
        slot->length = val.length;
    } else if (dup == INS_DUP_IGNORE) {
        // keep the existing bucket
    } else if (dup == INS_DUP_RIGHT) {
        this->Insert(mid, &val, 1);
    } else {
        return -1;
    }
    return mid;
}
