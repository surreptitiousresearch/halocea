#include "dsVECTOR.h"

struct strmVID_BUFFER;       // boundary — strm subsystem video buffer (element is a pointer)
struct strmVID_BUF_INS_PRED; // boundary — ordering predicate (compares by pointer value here)

// dsVECTOR<strmVID_BUFFER*,8>::InsertSorted<strmVID_BUF_INS_PRED> @ 0x8269BD10
// Binary-search insert into an ascending-by-pointer vector of strmVID_BUFFER pointers. On a miss,
// splice `val` at the lower-bound index and return it. On a match at `mid`, `dup` selects the
// duplicate policy (the predicate here reduces to a raw pointer comparison in the compiled code).
template<>
template<>
int dsVECTOR<strmVID_BUFFER *, 8>::InsertSorted<strmVID_BUF_INS_PRED>(
    strmVID_BUFFER *const &val, const strmVID_BUF_INS_PRED &, INS_SORT dup)
{
    int hi = this->nElem;
    int lo = 0;
    if (!hi) {
        this->Insert(lo, &val, 1);
        return lo;
    }

    strmVID_BUFFER *key = val;
    int mid;
    strmVID_BUFFER **slot;
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
